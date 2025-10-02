# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import seaborn as sns
from collections import Counter
import numpy as np
import argparse

plt.style.use('seaborn-v0_8')
sns.set_palette("husl")

def read_data(filename):
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f if line.strip()]
    return lines

def analyze_ngrams(lines, n):
    ngrams = []
    for i in range(len(lines) - n + 1):
        ngram = tuple(lines[i:i + n])
        ngrams.append(ngram)

    counter = Counter(ngrams)
    return counter

def format_ngram_label(ngram):
    label = ''
    for i in range(len(ngram)):
        label += ' ( ' + str(ngram[i]) + ' ) '
        if i != len(ngram) - 1:
            label += ' -> '

    return label

def plot_single_ngram_stats(counter, n):
    if not counter:
        return

    plt.figure(figsize=(14, 8))

    most_common = counter.most_common(10)
    ngrams, counts = zip(*most_common)

    y_pos = np.arange(len(ngrams))
    colors = plt.cm.Set3(np.linspace(0, 1, len(ngrams)))
    bars = plt.barh(y_pos, counts, color=colors, alpha=0.7, edgecolor='black', linewidth=0.5)

    for i, (bar, count) in enumerate(zip(bars, counts)):
        width = bar.get_width()
        plt.text(width + max(counts) * 0.01, bar.get_y() + bar.get_height()/2,
                f'{count}', ha='left', va='center', fontsize=11, fontweight='bold')

    plt.yticks(y_pos)
    ngram_labels = [format_ngram_label(ngram) for ngram in ngrams]
    plt.gca().set_yticklabels(ngram_labels, fontsize=10)
    plt.xlabel('Frequency', fontsize=12, fontweight='bold')
    plt.title(f'Top {n}-grams (Top-{10})', fontsize=16, fontweight='bold')
    plt.grid(axis='x', alpha=0.3)
    plt.gca().invert_yaxis()

    total_sequences = sum(counter.values())
    unique_sequences = len(counter)
    plt.figtext(0.02, 0.02, f'Total sequences: {total_sequences}\nUnique sequences: {unique_sequences}',
                fontsize=10, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgray"))

    plt.tight_layout()
    plt.savefig(f'{n}_grams.png', dpi=300, bbox_inches='tight')

    plt.show()

def main(filename):
    lines = read_data(filename)

    ngram_counts = {}
    for n in range(1, 6):
        ngram_counts[n] = analyze_ngrams(lines, n)

    for n in range(1, 6):
        plot_single_ngram_stats(ngram_counts[n], n)

def parse_arguments():
    parser = argparse.ArgumentParser(description='Analyze n-grams from a text file and generate plots')
    parser.add_argument('filename', help='Input file containing the data')

    return parser.parse_args()

if __name__ == "__main__":
    args = parse_arguments()

    main(filename=args.filename)
