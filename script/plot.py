import json
import matplotlib.pyplot as plt
import os

'''
def plot_google_benchmark_median(json_path):
    # TODO: open the json file and do necessary processing to plot what is needed.

    plt.figure(figsize=(10, 6))

    plt.ylabel("y-label")
    plt.xlabel("x-label")
    plt.title("Google Benchmark Median Times")
    plt.xticks(rotation=45, ha="right")
    plt.tight_layout()
    # plots folder does not exist, create it
    if not os.path.exists("./plots"):
        os.makedirs("./plots")
    plt.savefig("./plots/plot1.png")

# Example usage: the input should stay the same. The output plot should be saved in the plots folder.
plot_google_benchmark_median("./logs/lab02.json")
'''

def plot_cache_hierarchy(json_path):

    # Load JSON data
    with open(json_path, 'r') as f:
        data = json.load(f)
    
    benchmarks = data['benchmarks']
    
    # Extract array sizes and times
    array_sizes = []
    times = []
    
    for benchmark in benchmarks:
        # Extract array size from benchmark name 
        name = benchmark['name']
        parts = name.split('/')
        array_size = int(parts[1])  
        
        # Extract time 
        time = benchmark['real_time']
        
        array_sizes.append(array_size)
        times.append(time)
    
    # Convert array sizes to bytes (assuming double = 8 bytes)
    array_sizes_bytes = [size * 8 for size in array_sizes]

    # Create the plot
    plt.figure(figsize=(10, 6))
    plt.plot(array_sizes_bytes, times, marker='o', linewidth=2, markersize=6)
    
    plt.xscale('log', base=2)
    
    # Labels and formatting
    plt.xlabel("Array Size (bytes)", fontsize=12)
    plt.ylabel("Time (microseconds)", fontsize=12)
    plt.title("Cache Hierarchy Analysis - Copy Benchmark", fontsize=14)
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    # Create plots directory if it doesn't exist
    if not os.path.exists("./plots"):
        os.makedirs("./plots")
    
    plt.savefig("./plots/cache_analysis.png", dpi=300)
    print("Cache hierarchy plot saved to ./plots/cache_analysis.png")
    plt.close()

def plot_tiling_comparison(json_path):

    # Load JSON data
    with open(json_path, 'r') as f:
        data = json.load(f)
    
    benchmarks = data['benchmarks']
    
    # Separate baseline and tiled benchmarks
    baseline_time = None
    tiled_time = None
    baseline_name = None
    tiled_name = None
    
    for benchmark in benchmarks:
        name = benchmark['name']
        time = benchmark['real_time']
        
        if 'TILED' in name:
            tiled_time = time
            tiled_name = name
        else:
            baseline_time = time
            baseline_name = name
    
    # Calculate speedup percentage
    if baseline_time and tiled_time:
        speedup_percent = ((baseline_time - tiled_time) / baseline_time) * 100
        speedup_ratio = baseline_time / tiled_time

     # Create bar chart
    plt.figure(figsize=(10, 6))
    
    labels = ['Baseline\n(evolve)', 'Tiled\n(evolve_tiled)']
    times = [baseline_time, tiled_time]
    colors = ['#FF6B6B', '#4ECDC4']
    
    bars = plt.bar(labels, times, color=colors, alpha=0.8, edgecolor='black')
    
    # Add value labels on bars
    for bar, time in zip(bars, times):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
            f'{time:.1f} ms',
            ha='center', va='bottom', fontsize=11, fontweight='bold')
    
    # Add speedup annotation
    plt.text(0.5, max(times) * 0.9,
        f'Speedup: {speedup_percent:.1f}%\n({speedup_ratio:.2f}x faster)',
        ha='center', fontsize=12, bbox=dict(boxstyle='round', 
        facecolor='yellow', alpha=0.3))
    
    plt.ylabel("Execution Time (milliseconds)", fontsize=12)
    plt.title("Tiling Performance Comparison - Heat Equation Solver", fontsize=14)
    plt.grid(True, axis='y', alpha=0.3)
    
    plt.tight_layout()
    
    # Create plots directory if it doesn't exist
    if not os.path.exists("./plots"):
        os.makedirs("./plots")
    
    plt.savefig("./plots/tiling_comparison.png", dpi=300)
    print("Tiling comparison plot saved to ./plots/tiling_comparison.png")
    plt.close()

if __name__ == "__main__":
    plot_cache_hierarchy("logs/lab02-part1.json")
    plot_tiling_comparison("logs/lab02-part2.json")
