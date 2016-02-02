from matplotlib.pyplot import *
from numpy import *
from random import *

# Ajustes
#tmax = 1000
#step = 0.5
#mu = 0.5
#sig = 0.2

alphas = 0.01
alphaf = 0.1
wsi = 0
wfi = 0
ys = 1.0
yf = 1.0

## ----
#t = arange(0, tmax, step)
wf = [wfi]
ws = [wsi]
we = [0]
wa = []

wa.append(0.00343551) # 0
wa.append(0.00554873) # 1
wa.append(0.00543277) # 2
wa.append(0.00655408) # 3
wa.append(0.00991793) # 4
wa.append(0.0118564) # 5
wa.append(0.0120288) # 6
wa.append(0.00332443) # 7
wa.append(0.00643051) # 8
wa.append(0.00437105) # 9
wa.append(0.00265096) # 10
wa.append(0.0072627) # 11
wa.append(0.00646848) # 12
wa.append(0.00610539) # 13
wa.append(0.0041744) # 14
wa.append(0.00426445) # 15
wa.append(0.00548906) # 16
wa.append(0.00536188) # 17
wa.append(0.00626113) # 18
wa.append(0.00568487) # 19
wa.append(0.00740416) # 20
wa.append(0.00406148) # 21
wa.append(0.00385037) # 22
wa.append(0.00299306) # 23
wa.append(0.00316732) # 24
wa.append(0.00247334) # 25
wa.append(0.00544211) # 26
wa.append(0.00365479) # 27
wa.append(0.00390652) # 28
wa.append(0.00355389) # 29
wa.append(0.0040267) # 30
wa.append(0.00382276) # 31
wa.append(0.00391077) # 32
wa.append(0.00283707) # 33
wa.append(0.00406753) # 34
wa.append(0.00395763) # 35
wa.append(0.00422147) # 36
wa.append(0.00407211) # 37
wa.append(0.00300895) # 38
wa.append(0.00419609) # 39
wa.append(0.00427643) # 40
wa.append(0.00374103) # 41
wa.append(0.00440775) # 42
wa.append(0.00374597) # 43
wa.append(0.00275122) # 44
wa.append(0.00267416) # 45
wa.append(0.00265593) # 46
wa.append(0.00376219) # 47
wa.append(0.00274337) # 48
wa.append(0.00346524) # 49
wa.append(0.00341906) # 50
wa.append(0.00381106) # 51
wa.append(0.00343246) # 52
wa.append(0.00390809) # 53
wa.append(0.00489144) # 54
wa.append(0.00341132) # 55
wa.append(0.00334199) # 56
wa.append(0.00332668) # 57
wa.append(0.00542685) # 58
wa.append(0.0060007) # 59
wa.append(0.00397731) # 60
wa.append(0.00615255) # 61
wa.append(0.00276352) # 62
wa.append(0.00708948) # 63
wa.append(0.00169112) # 64
wa.append(0.00166767) # 65
wa.append(0.00163986) # 66
wa.append(0.00159077) # 67
wa.append(0.00238076) # 68
wa.append(0.00348001) # 69
wa.append(0.00360801) # 70
wa.append(0.00410669) # 71
wa.append(0.00438892) # 72
wa.append(0.00441471) # 73
wa.append(0.00453282) # 74
wa.append(0.00409592) # 75
wa.append(0.00489397) # 76
wa.append(0.00481014) # 77
wa.append(0.00573964) # 78
wa.append(0.00512304) # 79
wa.append(0.00499629) # 80
wa.append(0.0057778) # 81
wa.append(0.00503747) # 82
wa.append(0.00573458) # 83
wa.append(0.00612852) # 84
wa.append(0.00589328) # 85
wa.append(0.00695996) # 86
wa.append(0.00611184) # 87
wa.append(0.00551347) # 88
wa.append(0.00132981) # 89
wa.append(0.00132981) # 90
wa.append(0.00132981) # 91
wa.append(0.00160417) # 92
wa.append(0.00248912) # 93
wa.append(0.00303544) # 94
wa.append(0.00463293) # 95
wa.append(0.00452377) # 96
wa.append(0.00470739) # 97
wa.append(0.00531533) # 98
wa.append(0.00493054) # 99
wa.append(0.00554781) # 100
wa.append(0.00564806) # 101
wa.append(0.0026643) # 102
wa.append(0.00340795) # 103
wa.append(0.00343864) # 104
wa.append(0.00369494) # 105
wa.append(0.00430406) # 106
wa.append(0.00395137) # 107
wa.append(0.00420757) # 108
wa.append(0.00452539) # 109
wa.append(0.00176403) # 110
wa.append(0.00337465) # 111
wa.append(0.00428437) # 112
wa.append(0.00582285) # 113
wa.append(0.00752846) # 114
wa.append(0.00967302) # 115
wa.append(0.00972) # 116
wa.append(0.0136508) # 117
wa.append(0.0171565) # 118
wa.append(0.00140441) # 119
wa.append(0.00153989) # 120
wa.append(0.00168112) # 121
wa.append(0.00184737) # 122
wa.append(0.00189773) # 123
wa.append(0.0018306) # 124
wa.append(0.00204233) # 125
wa.append(0.00231513) # 126
wa.append(0.00256413) # 127
wa.append(0.00459795) # 128
wa.append(0.00510432) # 129
wa.append(0.00512217) # 130
wa.append(0.00327908) # 131
wa.append(0.00536904) # 132
wa.append(0.00317798) # 133
wa.append(0.00505163) # 134
wa.append(0.00543312) # 135
wa.append(0.00333337) # 136
wa.append(0.00541487) # 137
wa.append(0.00342217) # 138
wa.append(0.00626974) # 139
wa.append(0.00345971) # 140
wa.append(0.00632022) # 141
wa.append(0.004201) # 142
wa.append(0.00676531) # 143
wa.append(0.00696408) # 144
wa.append(0.00644765) # 145
wa.append(0.00583198) # 146
wa.append(0.00591505) # 147
wa.append(0.00708959) # 148
wa.append(0.00766495) # 149
wa.append(0.00136809) # 150
wa.append(0.00241465) # 151
wa.append(0.0042408) # 152
wa.append(0.00421239) # 153
wa.append(0.0043779) # 154
wa.append(0.00547465) # 155
wa.append(0.00426319) # 156
wa.append(0.00227895) # 157
wa.append(0.00210108) # 158
wa.append(0.00251119) # 159
wa.append(0.00301309) # 160
wa.append(0.00299494) # 161
wa.append(0.00296613) # 162
wa.append(0.00298672) # 163
wa.append(0.00292974) # 164
wa.append(0.00287935) # 165
wa.append(0.00415778) # 166
wa.append(0.00276722) # 167
wa.append(0.00433936) # 168
wa.append(0.00405573) # 169
wa.append(0.00238769) # 170
wa.append(0.00221613) # 171
wa.append(0.00359687) # 172
wa.append(0.00418848) # 173
wa.append(0.00382125) # 174
wa.append(0.0057194) # 175
wa.append(0.00613795) # 176
wa.append(0.00366685) # 177
wa.append(0.00478789) # 178
wa.append(0.0044455) # 179
wa.append(0.00410948) # 180
wa.append(0.00417163) # 181
wa.append(0.00297799) # 182
wa.append(0.00182468) # 183
wa.append(0.00312743) # 184
wa.append(0.00455081) # 185
wa.append(0.00483832) # 186
wa.append(0.00485905) # 187
wa.append(0.00453142) # 188
wa.append(0.00528249) # 189
wa.append(0.0045407) # 190
wa.append(0.00473217) # 191
wa.append(0.00551149) # 192
wa.append(0.00539712) # 193
wa.append(0.00681445) # 194
wa.append(0.00604209) # 195
wa.append(0.00678922) # 196
wa.append(0.00832423) # 197
wa.append(0.00824767) # 198
wa.append(0.0100976) # 199
wa.append(0.0128156) # 200
wa.append(0.0133153) # 201
wa.append(0.000824242) # 202
wa.append(0.00197942) # 203
wa.append(0.00172628) # 204
wa.append(0.00178029) # 205
wa.append(0.00137373) # 206
wa.append(0.00132981) # 207
wa.append(0.00132981) # 208
wa.append(0.00132981) # 209
wa.append(0.00132981) # 210
wa.append(0.00132981) # 211
wa.append(0.00158417) # 212
wa.append(0.00177484) # 213
wa.append(0.00206093) # 214
wa.append(0.00256227) # 215
wa.append(0.0023831) # 216
wa.append(0.00234929) # 217
wa.append(0.00283518) # 218
wa.append(0.0028458) # 219
wa.append(0.00462001) # 220
wa.append(0.00527842) # 221
wa.append(0.00580158) # 222
wa.append(0.00556645) # 223
wa.append(0.00454548) # 224
wa.append(0.00643586) # 225
wa.append(0.00566238) # 226
wa.append(0.00336634) # 227

#for i in range(len(t)):
#    mean = mu*(max(1-exp(-(t[i]/5))-exp(-((950-t[i])/1)), 0))
#    #wa.append(mean)
#    wa.append(gauss(mean, sig))

for i in range(len(wa)-1):
    wf.append(wf[i]+alphaf*(yf*wa[i+1]-wf[i]))
    ws.append(ws[i]+alphas*(ys*wa[i+1]-ws[i]))
    we.append(max(0, 1-wf[i+1]/ws[i+1]))

t = arange(0, len(wa), 1)
#for i in range(len(wa)):
#    we.append(wf[i]-ws[i])

l = []
for i in range(len(wa)):
    l.append(0)
#l[111] = 0.01
#l[200] = 0.01
#l[300] = 0.01
#l[400] = 0.01
#l[511] = 0.01

for i in range(len(we)):
    if we[i] > 0:
        we[i] = 0.01

#print l

plot(t, wf, 'r', t, ws, 'b', t, wa, 'g', t, we, 'y', t, l, 'k')
show()
