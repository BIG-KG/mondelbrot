import matplotlib.pyplot as plt
import os
import sys
import math

def read_data(file_path):
    x_vals, y_vals = [], []
    with open(file_path, 'r') as file:
        for line in file:
            x, y = map(float, line.strip().split())
            x_vals.append(x)
            y_vals.append(y)
    return x_vals, y_vals

def plot_files(title, save_dir, directory, y_pow):
    plt.figure(figsize=(10, 6))
    file_paths = [os.path.join(directory, f) for f in os.listdir(directory) if f.endswith(".txt")]
    
    if not file_paths:
        print("No .txt files found in the directory.")
        return
    
    for file_path in file_paths:
        x_vals, y_vals = read_data(file_path)
        if y_pow != "1":
            y_pow = int(y_pow)
            for i in range(len(y_vals)):
                y_vals[i] = math.pow(y_vals[i], y_pow)
        plt.plot(x_vals, y_vals, label=os.path.basename(file_path))
    
    plt.xlabel("Number of elements")
    if y_pow == "1":
        plt.ylabel("Time, s")
    else:
        string = "Time in " + str(y_pow) + " pow, s"
        plt.ylabel(string)
    plt.title(title)
    plt.legend()
    plt.grid()
    
    os.makedirs(save_dir, exist_ok=True)
    save_path = os.path.join(save_dir, f"{title}.png")
    plt.savefig(save_path)

if __name__ == "__main__":
    title, save_dir, directory, y_pow = sys.argv[1:5]
    plot_files(title, save_dir, directory, y_pow)
