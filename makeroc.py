#!/bin/env python

import ROOT as r
import numpy as np
from sklearn import metrics

f = r.TFile("bdt_output_data.root")
sig_tree = f.Get("signal_tree")
bkg_tree = f.Get("background_tree")


def makeroc(xmin, xmax, name, title):
    labels = []
    mva_scores = []
    mva_rewgt_scores = []
    for event in sig_tree:
        if event.x < xmin or event.x > xmax: continue
        labels.append(2)
        mva_scores.append(event.mva)
        mva_rewgt_scores.append(event.mva_rewgt)
    for event in bkg_tree:
        if event.x < xmin or event.x > xmax: continue
        labels.append(1)
        mva_scores.append(event.mva)
        mva_rewgt_scores.append(event.mva_rewgt)

    labels = np.array(labels)
    mva_scores = np.array(mva_scores)
    mva_rewgt_scores = np.array(mva_rewgt_scores)
    fpr, tpr, thresholds = metrics.roc_curve(labels, mva_scores, pos_label=2)
    fpr_rewgt, tpr_rewgt, thresholds_rewgt = metrics.roc_curve(labels, mva_rewgt_scores, pos_label=2)

    import matplotlib.pyplot as plt
    plt.figure()
    lw = 2
    plt.plot(fpr, tpr, color='darkorange', lw=lw, label='BDT no-rewgt')
    plt.plot(fpr_rewgt, tpr_rewgt, color='aqua', lw=lw, label='BDT yes-rewgt')
    plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--')
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('ROC {}'.format(title))
    plt.legend(loc="lower right")
    plt.savefig('{}.png'.format(name))

if __name__ == "__main__":
    makeroc(0, 1, "plot_incl", "inclusive in x")
    makeroc(0, 0.1, "plot_00_01", "0 < x < 0.1")
    makeroc(0.1, 0.2, "plot_01_02", "0.1 < x < 0.2")
    makeroc(0.2, 0.3, "plot_02_03", "0.2 < x < 0.3")
    makeroc(0.3, 0.4, "plot_03_04", "0.3 < x < 0.4")
    makeroc(0.4, 0.5, "plot_04_05", "0.4 < x < 0.5")
    makeroc(0.5, 0.6, "plot_05_06", "0.5 < x < 0.6")
    makeroc(0.6, 0.7, "plot_06_07", "0.6 < x < 0.7")
    makeroc(0.7, 0.8, "plot_07_08", "0.7 < x < 0.8")
    makeroc(0.8, 0.9, "plot_08_09", "0.8 < x < 0.9")
    makeroc(0.9, 1.0, "plot_09_10", "0.9 < x < 1.0")
