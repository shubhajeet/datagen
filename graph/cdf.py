import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def create_cdf_plot(csv_file, output_png_file):
    # Load data from a CSV file
    data = pd.read_csv(csv_file, header=None)
    # Assuming your CSV file has a column named 'values' containing the data
    values = data[0].dropna()  # Drop missing values if any

    # Sort the data
    sorted_data = np.sort(values)

    # Calculate the CDF
    cdf = np.arange(1, len(sorted_data) + 1) / len(sorted_data)

    # Create the CDF plot
    plt.plot(sorted_data, cdf, marker='.', linestyle='none')
    plt.xlabel('Data Values')
    plt.ylabel('CDF')
    plt.title('Cumulative Distribution Function (CDF) Curve')

    # Save the plot to a PNG file
    plt.savefig(output_png_file)

    # Show the plot (optional)
    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Generate a CDF plot from a CSV file.')
    parser.add_argument('csv_file', type=str, help='Input CSV file path')
    parser.add_argument('output_png_file', type=str,
                        help='Output PNG file path')

    args = parser.parse_args()

    create_cdf_plot(args.csv_file, args.output_png_file)
