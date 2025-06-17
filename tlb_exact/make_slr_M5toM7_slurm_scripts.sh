#!/bin/sh

# Define directories
INSTANCE_DIR=~/work/sbrpod/instances
TARGETS_DIR=~/work/tlb/targets_slr
RESULTS_DIR=~/work/tlb/results
SCRIPTS_DIR=~/work/tlb/scripts
RUN_SCRIPT=run_slurm_scripts.sh
EXECUTABLE=build/exec_tlb

mkdir -p "$SCRIPTS_DIR"

script_count=0
model=6

budgets="0.01 0.02 0.03 0.04 0.05 0.10 0.15 0.20"

# For model 2 (or 6) you require 25G and 8 hours

for bud in $budgets; do
	for instance_file in "$INSTANCE_DIR"/*_n*_e100_0.txt; do
		[ -f "$instance_file" ] || continue

		filename=$(basename "$instance_file")
		base_name="${filename%.txt}"
		city_prefix=$(echo "$base_name" | cut -d'_' -f1)
		scenario_file="${INSTANCE_DIR}/${city_prefix}_e400.txt"

		re_file="bud_re_m${model}_${base_name}_${bud}.txt"
		output_file="bud_targets_m${model}_${base_name}_${bud}.txt"

		if [ -f "$RESULTS_DIR/$re_file" ]; then
			echo "Skipping $filename since $re_file exists."
			continue
		fi

		script_name="$SCRIPTS_DIR/bud_${base_name}_${bud}.sh"

		cat > "$script_name" <<EOF
#!/bin/sh -l
#SBATCH --mem=25G
#SBATCH --time=8:00:00
#SBATCH --account=def-cotej
#SBATCH --cpus-per-task=64
export OMP_NUM_THREADS=\$SLURM_CPUS_PER_TASK

$EXECUTABLE mcfp_solver=3 instance_format=trips \\
	instance_file=$INSTANCE_DIR/$filename \\
	scenarios_file_name=$scenario_file \\
	model=${model} \\
	budget=${bud} \\
	re_file=$RESULTS_DIR/${re_file} \\
	output_file=$TARGETS_DIR/${output_file} \\
	> output/bud_m${model}_${base_name}_${bud}.out 2> output/bud_m${model}_${base_name}_${bud}.err
EOF

		chmod +x "$script_name"
		echo "Created script: $script_name"
		((script_count++))
	done
done

# Create the master SLURM submission script
RUN_SCRIPT_PATH="./$RUN_SCRIPT"
echo "#!/bin/bash" > "$RUN_SCRIPT_PATH"
for script in "$SCRIPTS_DIR"/*.sh; do
	echo "sbatch $script" >> "$RUN_SCRIPT_PATH"
done
chmod +x "$RUN_SCRIPT_PATH"

echo "Total scripts created: $script_count"
echo "Created SLURM batch submission script: $RUN_SCRIPT_PATH"
