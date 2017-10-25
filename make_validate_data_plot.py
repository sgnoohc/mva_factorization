#!/bin/env python

import ROOT as r
from plottery import plottery as ply

f = r.TFile("bdt_input_data.root")

ply.plot_hist_2d(f.Get("sig_hist"), {"output_name":"sig_hist.png"})
ply.plot_hist_2d(f.Get("bkg_hist"), {"output_name":"bkg_hist.png"})
ply.plot_hist_2d(f.Get("sig_hist"), {"output_name":"sig_hist.pdf"})
ply.plot_hist_2d(f.Get("bkg_hist"), {"output_name":"bkg_hist.pdf"})
