'''
Usage:python plot.py RootFile.root label[optional]
Author: L. Dodd, UW Madison
'''

import ROOT
import CMS_lumi, tdrstyle
import array
from subprocess import Popen
from sys import argv, exit, stdout, stderr
# So things don't look like crap.
ROOT.gROOT.SetBatch(True)

#set the tdr style
tdrstyle.setTDRStyle()

#change the CMS_lumi variables (see CMS_lumi.py)
CMS_lumi.lumi_13TeV = "36.8 fb^{-1}"
CMS_lumi.writeExtraText = 1
CMS_lumi.extraText = "Preliminary"
CMS_lumi.lumi_sqrtS = "13 TeV" # used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
iPos = 11
if( iPos==0 ): CMS_lumi.relPosX = 0.12
H_ref = 800; 
W_ref = 800; 
W = W_ref
H  = H_ref
iPeriod = 4
# references for T, B, L, R
T = 0.08*H_ref
B = 0.12*H_ref 
L = 0.12*W_ref
R = 0.04*W_ref
canvas = ROOT.TCanvas("c2","c2",50,50,W,H)
canvas.SetFillColor(0)
canvas.SetBorderMode(0)
canvas.SetFrameFillStyle(0)
canvas.SetFrameBorderMode(0)
canvas.SetLeftMargin( L/W )
canvas.SetRightMargin( R/W )
canvas.SetTopMargin( T/H )
canvas.SetBottomMargin( B/H )
canvas.SetTickx(0)
canvas.SetTicky(0)


######## File #########
if len(argv) < 6:
   print 'Usage:python file.py RootFile.root channel=MuTau/signal (for directory saving) direc=mt_inclusive variable=mt_2 (for file saving) xaxis=M_{T} #tau+ME_T '
   exit()

infile = argv[1]
channel = argv[2]
direc= argv[3]
variable = argv[4]
xaxis = argv[5]
shift = argv[6]

ntuple_file = ROOT.TFile(infile)

######## LABEL & SAVE WHERE #########

if len(argv)>2:
   saveWhere='~/www/ztt/2016/'+channel+'/'
else:
   saveWhere=''

x1_l = 0.98
y1_l = 0.90
dx_l = 0.32
dy_l = 0.58
x0_l = x1_l-dx_l
y0_l = y1_l-dy_l

legend1 =  ROOT.TLegend(x0_l,y0_l,x1_l, y1_l,"","brNDC")
legend1.SetFillColor(ROOT.kWhite)
legend1.SetBorderSize(1)

hmax=0
num=[]
####################################
#Get NTUPLE                 #
#####################################
#allnames = ['Zprime600','Zprime800','Zprime1000','Zprime1200','Zprime1400','Zprime1700','Zprime2000','Zprime2500','Zprime600A400h','Zprime800A400h','Zprime800A500h','Zprime800A600h','Zprime1000A600h','Zprime1000A800h','Zprime1200A400h','Zprime1200A500h','Zprime1200A600h','Zprime1200A700h','Zprime1200A800h','Zprime1400A400h','Zprime1400A500h','Zprime1400A600h','Zprime1400A700h','Zprime1400A800h','Zprime1700A400h','Zprime1700A500h','Zprime1700A600h','Zprime1700A700h','Zprime1700A800h','Zprime2000A400h','Zprime2000A500h','Zprime2000A600h','Zprime2000A700h','Zprime2000A800h','Zprime2500A400h','Zprime2500A500h','Zprime2500A600h','Zprime2500A700h','Zprime2500A800h']
allnames = [variable+'_CMS_scale_'+shift+'_mt_13TeVUp',variable,variable+'_CMS_scale_'+shift+'_mt_13TeVDown']
names=[]
for name in allnames:
    names.append(name) 

colors = [ROOT.kBlue-9,ROOT.kCyan+3,ROOT.kBlue,ROOT.kPink-3,ROOT.kRed+2,ROOT.kRed-9,ROOT.kSpring+3,ROOT.kMagenta+3]
histos = []

for name in names:
    hname = name
    histos.append(ROOT.TH1F(hname,"",1000,0,1000)) 

#######COLOR FUCKERY####
fancylines = {'Up':2,'Down':3,'TT':1}
fancycolors = {'Up':colors[0],'Down':colors[1],'TT':colors[3]}
fuckcolors = []
fucklines = []
for name in names:
    for a  in fancycolors:
        fuckcolors.append(fancycolors[a])
    for b in fancylines:
        fucklines.append(fancylines[b])
#print fucklines
#print len(fucklines)
#print fuckcolors
#print len(fuckcolors)


for i in range(0,len(names)):
    histos[i] = ntuple_file.Get(direc+'/'+names[i])
    histos[i].SetLineColor(fuckcolors[i])
    histos[i].SetLineStyle(fucklines[i])
    histos[i].SetLineWidth(3)
    histos[i].GetXaxis().SetTitle(xaxis)
    num.append(histos[i].Integral())
    print num[i] 
    #histos[i].Rebin(2)
    legend1.AddEntry(histos[i], names[i])
    if hmax<histos[i].GetMaximum():
       hmax = histos[i].GetMaximum()

for hist in histos:
    hist.SetMaximum(hmax*1.8)
    if hist==names[0]:
       hist.Draw("HIST")
    else:
       hist.Draw("HIST,same")
errUp = (num[0]-num[1])/num[1]
errDown = (num[2]-num[1])/num[1]
CMS_lumi.extraText = "Up %1.3f, Down %1.3f" % (errUp,errDown)
#draw the lumi text on the canvas
CMS_lumi.CMS_lumi(canvas, iPeriod, iPos)

canvas.cd()
canvas.Update()
canvas.RedrawAxis()
frame = canvas.GetFrame()
frame.Draw()

legend1.Draw("same")
saveas=saveWhere+variable+'_'+xaxis+'.png'
saveas=saveWhere+variable+'_'+xaxis+'.pdf'
canvas.Update()
canvas.SaveAs(saveas)
canvas


#
