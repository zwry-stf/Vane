#include "XyphraDef.h"
#include "Xyphra.h"

static const char proggy_clean_ttf_compressed_data_base85[11980 + 1] =
"7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
"2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
"`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
"i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
"kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
"*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
"tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
"ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
"x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
"CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
"U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
"'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
"_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
"Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
"/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
"%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
"OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
"h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
"o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
"j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
"sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
"eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
"M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
"LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
"%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
"Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
"a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
"$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
"nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
"7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
"D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
"P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
"bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
"h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
"V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
"sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
"$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
"hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
"@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
"w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
"u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
"d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
"6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
"b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
"tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
"$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
"7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
"u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
"LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
"_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
"hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
"^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
"+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
"9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
"CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
"hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
"8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
"S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
"0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
"+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
"M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
"?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
"Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
"[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
"wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
"Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
"MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
"i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
"1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
"iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
"URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
"w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
"d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
"A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
"/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
"m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
"TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
"GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
"O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";

static const char* GetDefaultCompressedFontDataTTFBase85()
{
	return proggy_clean_ttf_compressed_data_base85;
}

void XyFontAtlas::ClearTexData()
{
	XY_ASSERT(!Locked && "Cannot modify a locked XyFontAtlas between NewFrame() and EndFrame/Render()!");
	if (TexPixelsAlpha8)
		free(TexPixelsAlpha8);
	if (TexPixelsRGBA32)
		free(TexPixelsRGBA32);
	TexPixelsAlpha8 = NULL;
	TexPixelsRGBA32 = NULL;
	TexPixelsUseColors = false;
}

void XyFontAtlas::ClearFonts()
{
	XY_ASSERT(!Locked && "Cannot modify a locked XyFontAtlas between NewFrame() and EndFrame/Render()!");
	Fonts.clear_delete();
	TexReady = false;
}

void XyFontAtlas::Clear()
{
	ClearInputData();
	ClearTexData();
	ClearFonts();
}

bool XyFontAtlas::Build()
{
	XY_ASSERT(!Locked && "Cannot modify a locked XyFontAtlas between NewFrame() and EndFrame/Render()!");

	// Default font is none are specified
	if (ConfigData.Size == 0)
		AddFontDefault();

	// Build
	return BuildWithStbTruetype();
}

void XyFontAtlas::GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
	// Build atlas on demand
	if (TexPixelsAlpha8 == NULL)
		Build();

	*out_pixels = TexPixelsAlpha8;
	if (out_width) *out_width = TexWidth;
	if (out_height) *out_height = TexHeight;
	if (out_bytes_per_pixel) *out_bytes_per_pixel = 1;
}

void XyFontAtlas::GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
	// Convert to RGBA32 format on demand
	// Although it is likely to be the most commonly used format, our font rendering is 1 channel / 8 bpp
	if (!TexPixelsRGBA32)
	{
		unsigned char* pixels = NULL;
		GetTexDataAsAlpha8(&pixels, NULL, NULL);
		if (pixels)
		{
			TexPixelsRGBA32 = (unsigned int*)malloc((size_t)TexWidth * (size_t)TexHeight * 4);
			const unsigned char* src = pixels;
			unsigned int* dst = TexPixelsRGBA32;
			for (int n = TexWidth * TexHeight; n > 0; n--)
				*dst++ = XY_COL32(255, 255, 255, (unsigned int)(*src++));
		}
	}

	*out_pixels = (unsigned char*)TexPixelsRGBA32;
	if (out_width) *out_width = TexWidth;
	if (out_height) *out_height = TexHeight;
	if (out_bytes_per_pixel) *out_bytes_per_pixel = 4;
}

XyFontAtlas::XyFontAtlas(_Xyphra* _instance)
{
	memset(this, 0, sizeof(*this));
	TexGlyphPadding = 1;
	PackIdMouseCursors = PackIdLines = -1;
	ShadowRectIds[0] = ShadowRectIds[1] = -1;
	ShadowTexConfig.SetupDefaults();
	instance = _instance;
}

XyFontAtlas::~XyFontAtlas()
{
	XY_ASSERT(!Locked && "Cannot modify a locked XyFontAtlas between NewFrame() and EndFrame/Render()!");
	Clear();
}

XyFont* XyFontAtlas::AddFont(const XyFontConfig* font_cfg)
{
	XY_ASSERT(!Locked && "Cannot modify a locked XyFontAtlas between NewFrame() and EndFrame/Render()!");
	XY_ASSERT(font_cfg->FontData != NULL && font_cfg->FontDataSize > 0);
	XY_ASSERT(font_cfg->SizePixels > 0.0f);

	// Create new font
	if (!font_cfg->MergeMode)
		Fonts.push_back(new XyFont(instance));
	else
		XY_ASSERT(!Fonts.empty() && "Cannot use MergeMode for the first font"); // When using MergeMode make sure that a font has already been added before. You can use Vane::renderer.GetIO().Fonts->AddFontDefault() to add the default imgui font.

	ConfigData.push_back(*font_cfg);
	XyFontConfig& new_font_cfg = ConfigData.back();
	if (new_font_cfg.DstFont == NULL)
		new_font_cfg.DstFont = Fonts.back();
	if (!new_font_cfg.FontDataOwnedByAtlas)
	{
		new_font_cfg.FontData = malloc(new_font_cfg.FontDataSize);
		new_font_cfg.FontDataOwnedByAtlas = true;
		memcpy(new_font_cfg.FontData, font_cfg->FontData, (size_t)new_font_cfg.FontDataSize);
	}

	if (new_font_cfg.DstFont->EllipsisChar == (XyWchar)-1)
		new_font_cfg.DstFont->EllipsisChar = font_cfg->EllipsisChar;

	// Invalidate texture
	TexReady = false;
	ClearTexData();
	return new_font_cfg.DstFont;
}

XyFont* XyFontAtlas::AddFontDefault(const XyFontConfig* font_cfg_template)
{
	XyFontConfig font_cfg = font_cfg_template ? *font_cfg_template : XyFontConfig();
	if (!font_cfg_template)
	{
		font_cfg.OversampleH = font_cfg.OversampleV = 1;
		font_cfg.PixelSnapH = true;
	}
	if (font_cfg.SizePixels <= 0.0f)
		font_cfg.SizePixels = 13.0f * 1.0f;
	if (font_cfg.Name[0] == '\0')
		XyFormatString(font_cfg.Name, XY_ARRAYSIZE(font_cfg.Name), "ProggyClean.ttf, %dpx", (int)font_cfg.SizePixels);
	font_cfg.EllipsisChar = (XyWchar)0x0085;
	font_cfg.GlyphOffset.y = 1.0f * XY_FLOOR(font_cfg.SizePixels / 13.0f);  // Add +1 offset per 13 units

	const char* ttf_compressed_base85 = GetDefaultCompressedFontDataTTFBase85();
	const XyWchar* glyph_ranges = font_cfg.GlyphRanges != NULL ? font_cfg.GlyphRanges : GetGlyphRangesDefault();
	XyFont* font = AddFontFromMemoryCompressedBase85TTF(ttf_compressed_base85, font_cfg.SizePixels, &font_cfg, glyph_ranges);
	return font;
}

XyFont* XyFontAtlas::AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const XyFontConfig* font_cfg_template, const XyWchar* glyph_ranges)
{
	XY_ASSERT(!Locked && "Cannot modify a locked XyFontAtlas between NewFrame() and EndFrame/Render()!");
	XyFontConfig font_cfg = font_cfg_template ? *font_cfg_template : XyFontConfig();
	XY_ASSERT(font_cfg.FontData == NULL);
	font_cfg.FontData = ttf_data;
	font_cfg.FontDataSize = ttf_size;
	font_cfg.SizePixels = size_pixels > 0.0f ? size_pixels : font_cfg.SizePixels;
	if (glyph_ranges)
		font_cfg.GlyphRanges = glyph_ranges;
	return AddFont(&font_cfg);
}

static unsigned int stb_decompress_length(const unsigned char* input)
{
	return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char* stb__barrier_out_e, * stb__barrier_out_b;
static const unsigned char* stb__barrier_in_b;
static unsigned char* stb__dout;
static void stb__match(const unsigned char* data, unsigned int length)
{
	// INVERSE of memmove... write each byte before copying the next...
	XY_ASSERT(stb__dout + length <= stb__barrier_out_e);
	if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
	if (data < stb__barrier_out_b) { stb__dout = stb__barrier_out_e + 1; return; }
	while (length--) *stb__dout++ = *data++;
}

static void stb__lit(const unsigned char* data, unsigned int length)
{
	XY_ASSERT(stb__dout + length <= stb__barrier_out_e);
	if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
	if (data < stb__barrier_in_b) { stb__dout = stb__barrier_out_e + 1; return; }
	memcpy(stb__dout, data, length);
	stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static const unsigned char* stb_decompress_token(const unsigned char* i)
{
	if (*i >= 0x20) { // use fewer if's for cases that expand small
		if (*i >= 0x80)       stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
		else if (*i >= 0x40)  stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
		else /* *i >= 0x20 */ stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
	}
	else { // more ifs for cases that expand large, since overhead is amortized
		if (*i >= 0x18)       stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
		else if (*i >= 0x10)  stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
		else if (*i >= 0x08)  stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
		else if (*i == 0x07)  stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
		else if (*i == 0x06)  stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
		else if (*i == 0x04)  stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
	}
	return i;
}


static unsigned int stb_adler32(unsigned int adler32, unsigned char* buffer, unsigned int buflen)
{
	const unsigned long ADLER_MOD = 65521;
	unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
	unsigned long blocklen = buflen % 5552;

	unsigned long i;
	while (buflen) {
		for (i = 0; i + 7 < blocklen; i += 8) {
			s1 += buffer[0], s2 += s1;
			s1 += buffer[1], s2 += s1;
			s1 += buffer[2], s2 += s1;
			s1 += buffer[3], s2 += s1;
			s1 += buffer[4], s2 += s1;
			s1 += buffer[5], s2 += s1;
			s1 += buffer[6], s2 += s1;
			s1 += buffer[7], s2 += s1;

			buffer += 8;
		}

		for (; i < blocklen; ++i)
			s1 += *buffer++, s2 += s1;

		s1 %= ADLER_MOD, s2 %= ADLER_MOD;
		buflen -= blocklen;
		blocklen = 5552;
	}
	return (unsigned int)(s2 << 16) + (unsigned int)s1;
}

static unsigned int stb_decompress(unsigned char* output, const unsigned char* i, unsigned int length)
{
	if (stb__in4(0) != 0x57bC0000) return 0;
	if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
	const unsigned int olen = stb_decompress_length(i);
	stb__barrier_in_b = i;
	stb__barrier_out_e = output + olen;
	stb__barrier_out_b = output;
	i += 16;

	stb__dout = output;
	for (;;) {
		const unsigned char* old_i = i;
		i = stb_decompress_token(i);
		if (i == old_i) {
			if (*i == 0x05 && i[1] == 0xfa) {
				XY_ASSERT(stb__dout == output + olen);
				if (stb__dout != output + olen) return 0;
				if (stb_adler32(1, output, olen) != (unsigned int)stb__in4(2))
					return 0;
				return olen;
			}
			else {
				XY_ASSERT(0); /* NOTREACHED */
				return 0;
			}
		}
		XY_ASSERT(stb__dout <= output + olen);
		if (stb__dout > output + olen)
			return 0;
	}
}
static unsigned int Decode85Byte(char c) { return c >= '\\' ? c - 36 : c - 35; }
static void         Decode85(const unsigned char* src, unsigned char* dst)
{
	while (*src)
	{
		unsigned int tmp = Decode85Byte(src[0]) + 85 * (Decode85Byte(src[1]) + 85 * (Decode85Byte(src[2]) + 85 * (Decode85Byte(src[3]) + 85 * Decode85Byte(src[4]))));
		dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);   // We can't assume little-endianness.
		src += 5;
		dst += 4;
	}
}

XyFont* XyFontAtlas::AddFontFromMemoryCompressedTTF(const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const XyFontConfig* font_cfg_template, const XyWchar* glyph_ranges)
{
	const unsigned int buf_decompressed_size = stb_decompress_length((const unsigned char*)compressed_ttf_data);
	unsigned char* buf_decompressed_data = (unsigned char*)malloc(buf_decompressed_size);
	stb_decompress(buf_decompressed_data, (const unsigned char*)compressed_ttf_data, (unsigned int)compressed_ttf_size);

	XyFontConfig font_cfg = font_cfg_template ? *font_cfg_template : XyFontConfig();
	XY_ASSERT(font_cfg.FontData == NULL);
	font_cfg.FontDataOwnedByAtlas = true;
	return AddFontFromMemoryTTF(buf_decompressed_data, (int)buf_decompressed_size, size_pixels, &font_cfg, glyph_ranges);
}

XyFont* XyFontAtlas::AddFontFromMemoryCompressedBase85TTF(const char* compressed_ttf_data_base85, float size_pixels, const XyFontConfig* font_cfg, const XyWchar* glyph_ranges)
{
	int compressed_ttf_size = (((int)strlen(compressed_ttf_data_base85) + 4) / 5) * 4;
	void* compressed_ttf = malloc((size_t)compressed_ttf_size);
	Decode85((const unsigned char*)compressed_ttf_data_base85, (unsigned char*)compressed_ttf);
	XyFont* font = AddFontFromMemoryCompressedTTF(compressed_ttf, compressed_ttf_size, size_pixels, font_cfg, glyph_ranges);
	free(compressed_ttf);
	return font;
}

void XyFontAtlas::ClearInputData()
{
	XY_ASSERT(!Locked && "Cannot modify a locked XyFontAtlas between NewFrame() and EndFrame/Render()!");
	for (int i = 0; i < ConfigData.Size; i++)
		if (ConfigData[i].FontData && ConfigData[i].FontDataOwnedByAtlas)
		{
			free(ConfigData[i].FontData);
			ConfigData[i].FontData = NULL;
		}

	// When clearing this we lose access to the font name and other information used to build the font.
	for (int i = 0; i < Fonts.Size; i++)
		if (Fonts[i]->ConfigData >= ConfigData.Data && Fonts[i]->ConfigData < ConfigData.Data + ConfigData.Size)
		{
			Fonts[i]->ConfigData = NULL;
			Fonts[i]->ConfigDataCount = 0;
		}
	ConfigData.clear();
	CustomRects.clear();
	PackIdMouseCursors = PackIdLines = -1;
	ShadowRectIds[0] = ShadowRectIds[1] = -1;
	// Important: we leave TexReady untouched
}

int XyFontAtlas::AddCustomRectRegular(int width, int height)
{
	XY_ASSERT(width > 0 && width <= 0xFFFF);
	XY_ASSERT(height > 0 && height <= 0xFFFF);
	XyFontAtlasCustomRect r;
	r.Width = (unsigned short)width;
	r.Height = (unsigned short)height;
	CustomRects.push_back(r);
	return CustomRects.Size - 1; // Return index
}

int XyFontAtlas::AddCustomRectFontGlyph(XyFont* font, XyWchar id, int width, int height, float advance_x, const XyVec2& offset)
{
	XY_ASSERT(font != NULL);
	XY_ASSERT(width > 0 && width <= 0xFFFF);
	XY_ASSERT(height > 0 && height <= 0xFFFF);
	XyFontAtlasCustomRect r;
	r.Width = (unsigned short)width;
	r.Height = (unsigned short)height;
	r.GlyphID = id;
	r.GlyphAdvanceX = advance_x;
	r.GlyphOffset = offset;
	r.Font = font;
	CustomRects.push_back(r);
	return CustomRects.Size - 1; // Return index
}

void XyFontAtlas::CalcCustomRectUV(const XyFontAtlasCustomRect* rect, XyVec2* out_uv_min, XyVec2* out_uv_max) const
{
	XY_ASSERT(TexWidth > 0 && TexHeight > 0);   // Font atlas needs to be built before we can calculate UV coordinates
	XY_ASSERT(rect->IsPacked());                // Make sure the rectangle has been packed
	*out_uv_min = XyVec2((float)rect->X * TexUvScale.x, (float)rect->Y * TexUvScale.y);
	*out_uv_max = XyVec2((float)(rect->X + rect->Width) * TexUvScale.x, (float)(rect->Y + rect->Height) * TexUvScale.y);
}

XyFont::XyFont(_Xyphra* _instance)
{
	this->instance = _instance;
	FontSize = 0.0f;
	FallbackAdvanceX = 0.0f;
	FallbackChar = (XyWchar)-1;
	EllipsisChar = (XyWchar)-1;
	EllipsisWidth = EllipsisCharStep = 0.0f;
	EllipsisCharCount = 0;
	FallbackGlyph = NULL;
	ContainerAtlas = NULL;
	ConfigData = NULL;
	ConfigDataCount = 0;
	DirtyLookupTables = false;
	Scale = 1.0f;
	Ascent = Descent = 0.0f;
	MetricsTotalSurface = 0;
	memset(Used4kPagesMap, 0, sizeof(Used4kPagesMap));
}

XyFont::~XyFont()
{
	ClearOutputData();
}

const XyFontGlyph* XyFont::FindGlyph(XyWchar c) const
{
	if (c >= (size_t)IndexLookup.Size)
		return FallbackGlyph;
	const XyWchar i = IndexLookup.Data[c];
	if (i == (XyWchar)-1)
		return FallbackGlyph;
	return &Glyphs.Data[i];
}

const XyFontGlyph* XyFont::FindGlyphNoFallback(XyWchar c) const
{
	if (c >= (size_t)IndexLookup.Size)
		return NULL;
	const XyWchar i = IndexLookup.Data[c];
	if (i == (XyWchar)-1)
		return NULL;
	return &Glyphs.Data[i];
}


static inline const char* CalcWordWrapNextLineStartA(const char* text, const char* text_end)
{
	while (text < text_end && XyCharIsBlankA(*text))
		text++;
	if (*text == '\n')
		text++;
	return text;
}

XyVec2 XyFont::CalcTextWidthA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining) const
{
	if (!text_end)
		text_end = text_begin + strlen(text_begin); // FIXME-OPT: Need to avoid this.

	const float line_height = size;
	const float scale = size / FontSize;

	XyVec2 text_size = XyVec2(0, 0);
	float line_width = 0.0f;

	const bool word_wrap_enabled = (wrap_width > 0.0f);
	const char* word_wrap_eol = NULL;

	const char* s = text_begin;
	while (s < text_end)
	{
		if (word_wrap_enabled)
		{
			// Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
			if (!word_wrap_eol)
				word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - line_width);

			if (s >= word_wrap_eol)
			{
				if (text_size.x < line_width)
					text_size.x = line_width;
				text_size.y += line_height;
				line_width = 0.0f;
				word_wrap_eol = NULL;
				s = CalcWordWrapNextLineStartA(s, text_end); // Wrapping skips upcoming blanks
				continue;
			}
		}

		// Decode and advance source
		const char* prev_s = s;
		unsigned int c = (unsigned int)*s;
		if (c < 0x80)
			s += 1;
		else
			s += XyTextCharFromUtf8(&c, s, text_end);

		if (c < 32)
		{
			if (c == '\n')
			{
				text_size.x = XyMax(text_size.x, line_width);
				text_size.y += line_height;
				line_width = 0.0f;
				continue;
			}
			if (c == '\r')
				continue;
		}

		const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX.Data[c] : FallbackAdvanceX) * scale;
		if (line_width + char_width >= max_width)
		{
			s = prev_s;
			break;
		}

		line_width += char_width;
	}

	if (text_size.x < line_width)
		text_size.x = line_width;

	if (line_width > 0 || text_size.y == 0.0f)
		text_size.y += line_height;

	if (remaining)
		*remaining = s;

	return text_size;
}

const char* XyFont::CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const
{
	// For references, possible wrap point marked with ^
	//  "aaa bbb, ccc,ddd. eee   fff. ggg!"
	//      ^    ^    ^   ^   ^__    ^    ^

	// List of hardcoded separators: .,;!?'"

	// Skip extra blanks after a line returns (that includes not counting them in width computation)
	// e.g. "Hello    world" --> "Hello" "World"

	// Cut words that cannot possibly fit within one line.
	// e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"
	float line_width = 0.0f;
	float word_width = 0.0f;
	float blank_width = 0.0f;
	wrap_width /= scale; // We work with unscaled widths to avoid scaling every characters

	const char* word_end = text;
	const char* prev_word_end = NULL;
	bool inside_word = true;

	const char* s = text;
	XY_ASSERT(text_end != NULL);
	while (s < text_end)
	{
		unsigned int c = (unsigned int)*s;
		const char* next_s;
		if (c < 0x80)
			next_s = s + 1;
		else
			next_s = s + XyTextCharFromUtf8(&c, s, text_end);

		if (c < 32)
		{
			if (c == '\n')
			{
				line_width = word_width = blank_width = 0.0f;
				inside_word = true;
				s = next_s;
				continue;
			}
			if (c == '\r')
			{
				s = next_s;
				continue;
			}
		}

		const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX.Data[c] : FallbackAdvanceX);
		if (XyCharIsBlankW(c))
		{
			if (inside_word)
			{
				line_width += blank_width;
				blank_width = 0.0f;
				word_end = s;
			}
			blank_width += char_width;
			inside_word = false;
		}
		else
		{
			word_width += char_width;
			if (inside_word)
			{
				word_end = next_s;
			}
			else
			{
				prev_word_end = word_end;
				line_width += word_width + blank_width;
				word_width = blank_width = 0.0f;
			}

			// Allow wrapping after punctuation.
			inside_word = (c != '.' && c != ',' && c != ';' && c != '!' && c != '?' && c != '\"');
		}

		// We ignore blank width at the end of the line (they can be skipped)
		if (line_width + word_width > wrap_width)
		{
			// Words that cannot possibly fit within an entire line will be cut anywhere.
			if (word_width < wrap_width)
				s = prev_word_end ? prev_word_end : word_end;
			break;
		}

		s = next_s;
	}

	// Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
	// +1 may not be a character start point in UTF-8 but it's ok because caller loops use (text >= word_wrap_eol).
	if (s == text && text < text_end)
		return s + 1;
	return s;
}

void XyFont::RenderChar(float size, const XyVec2& pos, XyU32 col, XyWchar c) const
{
	const XyFontGlyph* glyph = FindGlyph(c);
	if (!glyph || !glyph->Visible)
		return;
	if (glyph->Colored)
		col |= ~XY_COL32_A_MASK;
	float scale = (size >= 0.0f) ? (size / FontSize) : 1.0f;
	float x = XY_FLOOR(pos.x);
	float y = XY_FLOOR(pos.y);
	instance->PrimReserve(6, 4);
	instance->PrimRectUV(XyVec2(x + glyph->X0 * scale, y + glyph->Y0 * scale), XyVec2(x + glyph->X1 * scale, y + glyph->Y1 * scale), XyVec2(glyph->U0, glyph->V0), XyVec2(glyph->U1, glyph->V1), col);
}

float XyFont::RenderTextTest(float size, const XyVec2& pos, XyU32 col, const XyVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip, float pos_find, void* out, bool search_type) const
{
	if (!text_end)
		text_end = text_begin + strlen(text_begin); // XyGui:: functions generally already provides a valid text_end, so this is merely to handle direct calls.

	// We dont do this, it looks ass LMAO
	// Align to be pixel perfect
	float x = pos.x; // XY_FLOOR(pos.x);
	float y = pos.y; // XY_FLOOR(pos.y);
	if (y > clip_rect.w)
		return x;

	const float start_x = x;
	const float scale = size / FontSize;
	const float line_height = FontSize * scale;
	const bool word_wrap_enabled = (wrap_width > 0.0f);

	// Fast-forward to first visible line
	const char* s = text_begin;
	if (y + line_height < clip_rect.y)
		while (y + line_height < clip_rect.y && s < text_end)
		{
			const char* line_end = (const char*)memchr(s, '\n', text_end - s);
			if (word_wrap_enabled)
			{
				// FIXME-OPT: This is not optimal as do first do a search for \n before calling CalcWordWrapPositionA().
				// If the specs for CalcWordWrapPositionA() were reworked to optionally return on \n we could combine both.
				// However it is still better than nothing performing the fast-forward!
				s = CalcWordWrapPositionA(scale, s, line_end ? line_end : text_end, wrap_width);
				s = CalcWordWrapNextLineStartA(s, text_end);
			}
			else
			{
				s = line_end ? line_end + 1 : text_end;
			}
			y += line_height;
		}

	// For large text, scan for the last visible line in order to avoid over-reserving in the call to PrimReserve()
	// Note that very large horizontal line will still be affected by the issue (e.g. a one megabyte string buffer without a newline will likely crash atm)
	if (text_end - s > 10000 && !word_wrap_enabled)
	{
		const char* s_end = s;
		float y_end = y;
		while (y_end < clip_rect.w && s_end < text_end)
		{
			s_end = (const char*)memchr(s_end, '\n', text_end - s_end);
			s_end = s_end ? s_end + 1 : text_end;
			y_end += line_height;
		}
		text_end = s_end;
	}
	if (s == text_end)
		return x;

	// Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
	const int vtx_count_max = (int)(text_end - s) * 4;
	const int idx_count_max = (int)(text_end - s) * 6;
	const int idx_expected_size = instance->IdxBuffer.Size + idx_count_max;
	instance->PrimReserve(idx_count_max, vtx_count_max);
	//XyVertex* instance->_VtxWritePtr = instance->_VtxWritePtr;
	//XyIndex* instance->_IdxWritePtr = instance->_IdxWritePtr;
	unsigned int vtx_index = instance->_VtxCurrentIdx;

	const XyU32 col_untinted = col | ~XY_COL32_A_MASK;
	const char* word_wrap_eol = NULL;

	int curr = 0;
	bool found = false;
	while (s < text_end)
	{
		if (word_wrap_enabled)
		{
			// Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
			if (!word_wrap_eol)
				word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - (x - start_x));

			if (s >= word_wrap_eol)
			{
				x = start_x;
				y += line_height;
				word_wrap_eol = NULL;
				s = CalcWordWrapNextLineStartA(s, text_end); // Wrapping skips upcoming blanks
				continue;
			}
		}

		// Decode and advance source
		unsigned int c = (unsigned int)*s;
		if (c < 0x80)
			s += 1;
		else
			s += XyTextCharFromUtf8(&c, s, text_end);

		if (c < 32)
		{
			if (c == '\n')
			{
				x = start_x;

				y += line_height;
				if (y > clip_rect.w)
					break; // break out of main loop
				continue;
			}
			if (c == '\r')
				continue;
		}
		const XyFontGlyph* glyph = FindGlyph((XyWchar)c);
		if (glyph == NULL)
			continue;

		float char_width = glyph->AdvanceX * scale;
		if (glyph->Visible)
		{
			// We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.y and exit once we pass clip_rect.w
			float x1 = x + glyph->X0 * scale;
			float x2 = x + glyph->X1 * scale;
			float y1 = y + glyph->Y0 * scale;
			float y2 = y + glyph->Y1 * scale;
			if (x1 <= clip_rect.z && x2 >= clip_rect.x)
			{
				// Render a character
				float u1 = glyph->U0;
				float v1 = glyph->V0;
				float u2 = glyph->U1;
				float v2 = glyph->V1;

				// CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
				if (cpu_fine_clip)
				{
					if (x1 < clip_rect.x)
					{
						u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
						x1 = clip_rect.x;
					}
					if (y1 < clip_rect.y)
					{
						v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
						y1 = clip_rect.y;
					}
					if (x2 > clip_rect.z)
					{
						u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
						x2 = clip_rect.z;
					}
					if (y2 > clip_rect.w)
					{
						v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
						y2 = clip_rect.w;
					}
					if (y1 >= y2)
					{
						x += char_width;
						continue;
					}
				}

				// Support for untinted glyphs
				XyU32 glyph_col = glyph->Colored ? col_untinted : col;

				// We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug builds. Inlined here:
				{
					instance->_VtxWritePtr[0].pos.x = x1; instance->_VtxWritePtr[0].pos.y = y1; instance->_VtxWritePtr[0].col = glyph_col; instance->_VtxWritePtr[0].uv.x = u1; instance->_VtxWritePtr[0].uv.y = v1;
					instance->_VtxWritePtr[1].pos.x = x2; instance->_VtxWritePtr[1].pos.y = y1; instance->_VtxWritePtr[1].col = glyph_col; instance->_VtxWritePtr[1].uv.x = u2; instance->_VtxWritePtr[1].uv.y = v1;
					instance->_VtxWritePtr[2].pos.x = x2; instance->_VtxWritePtr[2].pos.y = y2; instance->_VtxWritePtr[2].col = glyph_col; instance->_VtxWritePtr[2].uv.x = u2; instance->_VtxWritePtr[2].uv.y = v2;
					instance->_VtxWritePtr[3].pos.x = x1; instance->_VtxWritePtr[3].pos.y = y2; instance->_VtxWritePtr[3].col = glyph_col; instance->_VtxWritePtr[3].uv.x = u1; instance->_VtxWritePtr[3].uv.y = v2;
					instance->_IdxWritePtr[0] = (XyIndex)(vtx_index); instance->_IdxWritePtr[1] = (XyIndex)(vtx_index + 1); instance->_IdxWritePtr[2] = (XyIndex)(vtx_index + 2);
					instance->_IdxWritePtr[3] = (XyIndex)(vtx_index); instance->_IdxWritePtr[4] = (XyIndex)(vtx_index + 2); instance->_IdxWritePtr[5] = (XyIndex)(vtx_index + 3);
					instance->_VtxWritePtr += 4;
					vtx_index += 4;
					instance->_IdxWritePtr += 6;
				}
			}
		}
		x += char_width;

		if (!found) 
		{ // search_type == 1: by index, 0: by position
			if (search_type && curr == (int)pos_find)
			{
				found = true;
				*(float*)out = x - char_width;
			}
			else if (!search_type && x > pos_find)
			{
				found = true;
				*(int*)out = curr;
			}
		}
		curr++;
	}

	// If our pos is after the last character, we use last character
	if (search_type) // by index
	{
		if (!found && (int)pos_find >= 0)
			*(float*)out = x;
	}
	else // by position
	{
		if (!found && pos_find >= pos.x)
			*(int*)out = curr;
	}

	// Give back unused vertices (clipped ones, blanks) ~ this is essentially a PrimUnreserve() action.
	instance->VtxBuffer.Size = (int)(instance->_VtxWritePtr - instance->VtxBuffer.Data); // Same as calling shrink()
	instance->IdxBuffer.Size = (int)(instance->_IdxWritePtr - instance->IdxBuffer.Data);
	instance->CmdBuffer[instance->CmdBuffer.Size - 1].ElemCount -= (idx_expected_size - instance->IdxBuffer.Size);
	instance->_VtxWritePtr = instance->_VtxWritePtr;
	instance->_IdxWritePtr = instance->_IdxWritePtr;
	instance->_VtxCurrentIdx = vtx_index;

	return x;
}

float XyFont::RenderText(float size, const XyVec2& pos, XyU32 col, const XyVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip) const
{
	if (!text_end)
		text_end = text_begin + strlen(text_begin); // XyGui:: functions generally already provides a valid text_end, so this is merely to handle direct calls.

	/// We dont do this, it looks ass LMAO
	// Align to be pixel perfect
	float x = pos.x; // XY_FLOOR(pos.x);
	float y = pos.y; // XY_FLOOR(pos.y);
	if (y > clip_rect.w)
		return x;

	const float start_x = x;
	const float scale = size / FontSize;
	const float line_height = FontSize * scale;
	const bool word_wrap_enabled = (wrap_width > 0.0f);

	// Fast-forward to first visible line
	const char* s = text_begin;
	if (y + line_height < clip_rect.y)
		while (y + line_height < clip_rect.y && s < text_end)
		{
			const char* line_end = (const char*)memchr(s, '\n', text_end - s);
			if (word_wrap_enabled)
			{
				// FIXME-OPT: This is not optimal as do first do a search for \n before calling CalcWordWrapPositionA().
				// If the specs for CalcWordWrapPositionA() were reworked to optionally return on \n we could combine both.
				// However it is still better than nothing performing the fast-forward!
				s = CalcWordWrapPositionA(scale, s, line_end ? line_end : text_end, wrap_width);
				s = CalcWordWrapNextLineStartA(s, text_end);
			}
			else
			{
				s = line_end ? line_end + 1 : text_end;
			}
			y += line_height;
		}

	// For large text, scan for the last visible line in order to avoid over-reserving in the call to PrimReserve()
	// Note that very large horizontal line will still be affected by the issue (e.g. a one megabyte string buffer without a newline will likely crash atm)
	if (text_end - s > 10000 && !word_wrap_enabled)
	{
		const char* s_end = s;
		float y_end = y;
		while (y_end < clip_rect.w && s_end < text_end)
		{
			s_end = (const char*)memchr(s_end, '\n', text_end - s_end);
			s_end = s_end ? s_end + 1 : text_end;
			y_end += line_height;
		}
		text_end = s_end;
	}
	if (s == text_end)
		return x;

	// Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
	const int vtx_count_max = (int)(text_end - s) * 4;
	const int idx_count_max = (int)(text_end - s) * 6;
	const int idx_expected_size = instance->IdxBuffer.Size + idx_count_max;
	instance->PrimReserve(idx_count_max, vtx_count_max);
	//XyVertex* instance->_VtxWritePtr = instance->_VtxWritePtr;
	//XyIndex* instance->_IdxWritePtr = instance->_IdxWritePtr;
	unsigned int vtx_index = instance->_VtxCurrentIdx;

	const XyU32 col_untinted = col | ~XY_COL32_A_MASK;
	const char* word_wrap_eol = NULL;

	while (s < text_end)
	{
		if (word_wrap_enabled)
		{
			// Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
			if (!word_wrap_eol)
				word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - (x - start_x));

			if (s >= word_wrap_eol)
			{
				x = start_x;
				y += line_height;
				word_wrap_eol = NULL;
				s = CalcWordWrapNextLineStartA(s, text_end); // Wrapping skips upcoming blanks
				continue;
			}
		}

		// Decode and advance source
		unsigned int c = (unsigned int)*s;
		if (c < 0x80)
			s += 1;
		else
			s += XyTextCharFromUtf8(&c, s, text_end);

		if (c < 32)
		{
			if (c == '\n')
			{
				x = start_x;
				y += line_height;
				if (y > clip_rect.w)
					break; // break out of main loop
				continue;
			}
			if (c == '\r')
				continue;
		}

		const XyFontGlyph* glyph = FindGlyph((XyWchar)c);
		if (glyph == NULL)
			continue;

		float char_width = glyph->AdvanceX * scale;
		if (glyph->Visible)
		{
			// We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.y and exit once we pass clip_rect.w
			float x1 = x + glyph->X0 * scale;
			float x2 = x + glyph->X1 * scale;
			float y1 = y + glyph->Y0 * scale;
			float y2 = y + glyph->Y1 * scale;
			if (x1 <= clip_rect.z && x2 >= clip_rect.x)
			{
				// Render a character
				float u1 = glyph->U0;
				float v1 = glyph->V0;
				float u2 = glyph->U1;
				float v2 = glyph->V1;

				// CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
				if (cpu_fine_clip)
				{
					if (x1 < clip_rect.x)
					{
						u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
						x1 = clip_rect.x;
					}
					if (y1 < clip_rect.y)
					{
						v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
						y1 = clip_rect.y;
					}
					if (x2 > clip_rect.z)
					{
						u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
						x2 = clip_rect.z;
					}
					if (y2 > clip_rect.w)
					{
						v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
						y2 = clip_rect.w;
					}
					if (y1 >= y2)
					{
						x += char_width;
						continue;
					}
				}

				// Support for untinted glyphs
				XyU32 glyph_col = glyph->Colored ? col_untinted : col;

				// We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug builds. Inlined here:
				{
					instance->_VtxWritePtr[0].pos.x = x1; instance->_VtxWritePtr[0].pos.y = y1; instance->_VtxWritePtr[0].col = glyph_col; instance->_VtxWritePtr[0].uv.x = u1; instance->_VtxWritePtr[0].uv.y = v1;
					instance->_VtxWritePtr[1].pos.x = x2; instance->_VtxWritePtr[1].pos.y = y1; instance->_VtxWritePtr[1].col = glyph_col; instance->_VtxWritePtr[1].uv.x = u2; instance->_VtxWritePtr[1].uv.y = v1;
					instance->_VtxWritePtr[2].pos.x = x2; instance->_VtxWritePtr[2].pos.y = y2; instance->_VtxWritePtr[2].col = glyph_col; instance->_VtxWritePtr[2].uv.x = u2; instance->_VtxWritePtr[2].uv.y = v2;
					instance->_VtxWritePtr[3].pos.x = x1; instance->_VtxWritePtr[3].pos.y = y2; instance->_VtxWritePtr[3].col = glyph_col; instance->_VtxWritePtr[3].uv.x = u1; instance->_VtxWritePtr[3].uv.y = v2;
					instance->_IdxWritePtr[0] = (XyIndex)(vtx_index); instance->_IdxWritePtr[1] = (XyIndex)(vtx_index + 1); instance->_IdxWritePtr[2] = (XyIndex)(vtx_index + 2);
					instance->_IdxWritePtr[3] = (XyIndex)(vtx_index); instance->_IdxWritePtr[4] = (XyIndex)(vtx_index + 2); instance->_IdxWritePtr[5] = (XyIndex)(vtx_index + 3);
					instance->_VtxWritePtr += 4;
					vtx_index += 4;
					instance->_IdxWritePtr += 6;
				}
			}
		}
		x += char_width;
	}

	// Give back unused vertices (clipped ones, blanks) ~ this is essentially a PrimUnreserve() action.
	instance->VtxBuffer.Size = (int)(instance->_VtxWritePtr - instance->VtxBuffer.Data); // Same as calling shrink()
	instance->IdxBuffer.Size = (int)(instance->_IdxWritePtr - instance->IdxBuffer.Data);
	instance->CmdBuffer[instance->CmdBuffer.Size - 1].ElemCount -= (idx_expected_size - instance->IdxBuffer.Size);
	instance->_VtxWritePtr = instance->_VtxWritePtr;
	instance->_IdxWritePtr = instance->_IdxWritePtr;
	instance->_VtxCurrentIdx = vtx_index;

	return x;
}

#define XY_TABSIZE                      (4)
#define XY_UNICODE_CODEPOINT_INVALID 0xFFFD     // Invalid Unicode code point (standard value).
static XyWchar FindFirstExistingGlyph(XyFont* font, const XyWchar* candidate_chars, int candidate_chars_count)
{
	for (int n = 0; n < candidate_chars_count; n++)
		if (font->FindGlyphNoFallback(candidate_chars[n]) != NULL)
			return candidate_chars[n];
	return (XyWchar)-1;
}

void XyFont::BuildLookupTable()
{
	int max_codepoint = 0;
	for (int i = 0; i != Glyphs.Size; i++)
		max_codepoint = XyMax(max_codepoint, (int)Glyphs[i].Codepoint);

	// Build lookup table
	XY_ASSERT(Glyphs.Size < 0xFFFF); // -1 is reserved
	IndexAdvanceX.clear();
	IndexLookup.clear();
	DirtyLookupTables = false;
	memset(Used4kPagesMap, 0, sizeof(Used4kPagesMap));
	GrowIndex(max_codepoint + 1);
	for (int i = 0; i < Glyphs.Size; i++)
	{
		int codepoint = (int)Glyphs[i].Codepoint;
		IndexAdvanceX[codepoint] = Glyphs[i].AdvanceX;
		IndexLookup[codepoint] = (XyWchar)i;

		// Mark 4K page as used
		const int page_n = codepoint / 4096;
		Used4kPagesMap[page_n >> 3] |= 1 << (page_n & 7);
	}

	// Create a glyph to handle TAB
	// FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
	if (FindGlyph((XyWchar)' '))
	{
		if (Glyphs.back().Codepoint != '\t')   // So we can call this function multiple times (FIXME: Flaky)
			Glyphs.resize(Glyphs.Size + 1);
		XyFontGlyph& tab_glyph = Glyphs.back();
		tab_glyph = *FindGlyph((XyWchar)' ');
		tab_glyph.Codepoint = '\t';
		tab_glyph.AdvanceX *= XY_TABSIZE;
		IndexAdvanceX[(int)tab_glyph.Codepoint] = (float)tab_glyph.AdvanceX;
		IndexLookup[(int)tab_glyph.Codepoint] = (XyWchar)(Glyphs.Size - 1);
	}

	// Mark special glyphs as not visible (note that AddGlyph already mark as non-visible glyphs with zero-size polygons)
	SetGlyphVisible((XyWchar)' ', false);
	SetGlyphVisible((XyWchar)'\t', false);

	// Setup Fallback character
	const XyWchar fallback_chars[] = { (XyWchar)XY_UNICODE_CODEPOINT_INVALID, (XyWchar)'?', (XyWchar)' ' };
	FallbackGlyph = FindGlyphNoFallback(FallbackChar);
	if (FallbackGlyph == NULL)
	{
		FallbackChar = FindFirstExistingGlyph(this, fallback_chars, XY_ARRAYSIZE(fallback_chars));
		FallbackGlyph = FindGlyphNoFallback(FallbackChar);
		if (FallbackGlyph == NULL)
		{
			FallbackGlyph = &Glyphs.back();
			FallbackChar = (XyWchar)FallbackGlyph->Codepoint;
		}
	}
	FallbackAdvanceX = FallbackGlyph->AdvanceX;
	for (int i = 0; i < max_codepoint + 1; i++)
		if (IndexAdvanceX[i] < 0.0f)
			IndexAdvanceX[i] = FallbackAdvanceX;

	// Setup Ellipsis character. It is required for rendering elided text. We prefer using U+2026 (horizontal ellipsis).
	// However some old fonts may contain ellipsis at U+0085. Here we auto-detect most suitable ellipsis character.
	// FIXME: Note that 0x2026 is rarely included in our font ranges. Because of this we are more likely to use three individual dots.
	const XyWchar ellipsis_chars[] = { (XyWchar)0x2026, (XyWchar)0x0085 };
	const XyWchar dots_chars[] = { (XyWchar)'.', (XyWchar)0xFF0E };
	if (EllipsisChar == (XyWchar)-1)
		EllipsisChar = FindFirstExistingGlyph(this, ellipsis_chars, XY_ARRAYSIZE(ellipsis_chars));
	const XyWchar dot_char = FindFirstExistingGlyph(this, dots_chars, XY_ARRAYSIZE(dots_chars));
	if (EllipsisChar != (XyWchar)-1)
	{
		EllipsisCharCount = 1;
		EllipsisWidth = EllipsisCharStep = FindGlyph(EllipsisChar)->X1;
	}
	else if (dot_char != (XyWchar)-1)
	{
		const XyFontGlyph* glyph = FindGlyph(dot_char);
		EllipsisChar = dot_char;
		EllipsisCharCount = 3;
		EllipsisCharStep = (glyph->X1 - glyph->X0) + 1.0f;
		EllipsisWidth = EllipsisCharStep * 3.0f - 1.0f;
	}
}

void XyFont::ClearOutputData()
{
	FontSize = 0.0f;
	FallbackAdvanceX = 0.0f;
	Glyphs.clear();
	IndexAdvanceX.clear();
	IndexLookup.clear();
	FallbackGlyph = NULL;
	ContainerAtlas = NULL;
	DirtyLookupTables = true;
	Ascent = Descent = 0.0f;
	MetricsTotalSurface = 0;
}

void XyFont::GrowIndex(int new_size)
{
	XY_ASSERT(IndexAdvanceX.Size == IndexLookup.Size);
	if (new_size <= IndexLookup.Size)
		return;
	IndexAdvanceX.resize(new_size, -1.0f);
	IndexLookup.resize(new_size, (XyWchar)-1);
}

void XyFont::AddGlyph(const XyFontConfig* cfg, XyWchar codepoint, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x)
{
	if (cfg != NULL)
	{
		// Clamp & recenter if needed
		const float advance_x_original = advance_x;
		advance_x = XyClamp(advance_x, cfg->GlyphMinAdvanceX, cfg->GlyphMaxAdvanceX);
		if (advance_x != advance_x_original)
		{
			float char_off_x = cfg->PixelSnapH ? XyFloor((advance_x - advance_x_original) * 0.5f) : (advance_x - advance_x_original) * 0.5f;
			x0 += char_off_x;
			x1 += char_off_x;
		}

		// Snap to pixel
		if (cfg->PixelSnapH)
			advance_x = XY_ROUND(advance_x);

		// Bake spacing
		advance_x += cfg->GlyphExtraSpacing.x;
	}

	Glyphs.resize(Glyphs.Size + 1);
	XyFontGlyph& glyph = Glyphs.back();
	glyph.Codepoint = (unsigned int)codepoint;
	glyph.Visible = (x0 != x1) && (y0 != y1);
	glyph.Colored = false;
	glyph.X0 = x0;
	glyph.Y0 = y0;
	glyph.X1 = x1;
	glyph.Y1 = y1;
	glyph.U0 = u0;
	glyph.V0 = v0;
	glyph.U1 = u1;
	glyph.V1 = v1;
	glyph.AdvanceX = advance_x;

	// Compute rough surface usage metrics (+1 to account for average padding, +0.99 to round)
	// We use (U1-U0)*TexWidth instead of X1-X0 to account for oversampling.
	float pad = ContainerAtlas->TexGlyphPadding + 0.99f;
	DirtyLookupTables = true;
	MetricsTotalSurface += (int)((glyph.U1 - glyph.U0) * ContainerAtlas->TexWidth + pad) * (int)((glyph.V1 - glyph.V0) * ContainerAtlas->TexHeight + pad);
}

void XyFont::AddRemapChar(XyWchar dst, XyWchar src, bool overwrite_dst)
{
	XY_ASSERT(IndexLookup.Size > 0);    // Currently this can only be called AFTER the font has been built, aka after calling XyFontAtlas::GetTexDataAs*() function.
	unsigned int index_size = (unsigned int)IndexLookup.Size;

	if (dst < index_size && IndexLookup.Data[dst] == (XyWchar)-1 && !overwrite_dst) // 'dst' already exists
		return;
	if (src >= index_size && dst >= index_size) // both 'dst' and 'src' don't exist -> no-op
		return;

	GrowIndex(dst + 1);
	IndexLookup[dst] = (src < index_size) ? IndexLookup.Data[src] : (XyWchar)-1;
	IndexAdvanceX[dst] = (src < index_size) ? IndexAdvanceX.Data[src] : 1.0f;
}

void XyFont::SetGlyphVisible(XyWchar c, bool visible)
{
	if (XyFontGlyph* glyph = (XyFontGlyph*)(void*)FindGlyph((XyWchar)c))
		glyph->Visible = visible ? 1 : 0;
}

bool XyFont::IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last)
{
	unsigned int page_begin = (c_begin / 4096);
	unsigned int page_last = (c_last / 4096);
	for (unsigned int page_n = page_begin; page_n <= page_last; page_n++)
		if ((page_n >> 3) < sizeof(Used4kPagesMap))
			if (Used4kPagesMap[page_n >> 3] & (1 << (page_n & 7)))
				return false;
	return true;
}
const XyWchar* XyFontAtlas::GetGlyphRangesDefault()
{
	static const XyWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0,
	};
	return &ranges[0];
}

const XyWchar* XyFontAtlas::GetGlyphRangesGreek()
{
	static const XyWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0370, 0x03FF, // Greek and Coptic
		0,
	};
	return &ranges[0];
}

const XyWchar* XyFontAtlas::GetGlyphRangesKorean()
{
	static const XyWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x3131, 0x3163, // Korean alphabets
		0xAC00, 0xD7A3, // Korean characters
		0xFFFD, 0xFFFD, // Invalid
		0,
	};
	return &ranges[0];
}

const XyWchar* XyFontAtlas::GetGlyphRangesChineseFull()
{
	static const XyWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x2000, 0x206F, // General Punctuation
		0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
		0x31F0, 0x31FF, // Katakana Phonetic Extensions
		0xFF00, 0xFFEF, // Half-width characters
		0xFFFD, 0xFFFD, // Invalid
		0x4e00, 0x9FAF, // CJK Ideograms
		0,
	};
	return &ranges[0];
}


static void UnpackAccumulativeOffsetsIntoRanges(int base_codepoint, const short* accumulative_offsets, int accumulative_offsets_count, XyWchar* out_ranges)
{
	for (int n = 0; n < accumulative_offsets_count; n++, out_ranges += 2)
	{
		out_ranges[0] = out_ranges[1] = (XyWchar)(base_codepoint + accumulative_offsets[n]);
		base_codepoint += accumulative_offsets[n];
	}
	out_ranges[0] = 0;
}

const XyWchar* XyFontAtlas::GetGlyphRangesJapanese()
{
	// 2999 ideograms code points for Japanese
	// - 2136 Joyo (meaning "for regular use" or "for common use") Kanji code points
	// - 863 Jinmeiyo (meaning "for personal name") Kanji code points
	// - Sourced from official information provided by the government agencies of Japan:
	//   - List of Joyo Kanji by the Agency for Cultural Affairs
	//     - https://www.bunka.go.jp/kokugo_nihongo/sisaku/joho/joho/kijun/naikaku/kanji/
	//   - List of Jinmeiyo Kanji by the Ministry of Justice
	//     - http://www.moj.go.jp/MINJI/minji86.html
	//   - Available under the terms of the Creative Commons Attribution 4.0 International (CC BY 4.0).
	//     - https://creativecommons.org/licenses/by/4.0/legalcode
	// - You can generate this code by the script at:
	//   - https://github.com/vaiorabbit/everyday_use_kanji
	// - References:
	//   - List of Joyo Kanji
	//     - (Wikipedia) https://en.wikipedia.org/wiki/List_of_j%C5%8Dy%C5%8D_kanji
	//   - List of Jinmeiyo Kanji
	//     - (Wikipedia) https://en.wikipedia.org/wiki/Jinmeiy%C5%8D_kanji
	// - Missing 1 Joyo Kanji: U+20B9F (Kun'yomi: Shikaru, On'yomi: Shitsu,shichi), see https://github.com/ocornut/imgui/pull/3627 for details.
	// You can use ImFontGlyphRangesBuilder to create your own ranges derived from this, by merging existing ranges or adding new characters.
	// (Stored as accumulative offsets from the initial unicode codepoint 0x4E00. This encoding is designed to helps us compact the source code size.)
	static const short accumulative_offsets_from_0x4E00[] =
	{
		0,1,2,4,1,1,1,1,2,1,3,3,2,2,1,5,3,5,7,5,6,1,2,1,7,2,6,3,1,8,1,1,4,1,1,18,2,11,2,6,2,1,2,1,5,1,2,1,3,1,2,1,2,3,3,1,1,2,3,1,1,1,12,7,9,1,4,5,1,
		1,2,1,10,1,1,9,2,2,4,5,6,9,3,1,1,1,1,9,3,18,5,2,2,2,2,1,6,3,7,1,1,1,1,2,2,4,2,1,23,2,10,4,3,5,2,4,10,2,4,13,1,6,1,9,3,1,1,6,6,7,6,3,1,2,11,3,
		2,2,3,2,15,2,2,5,4,3,6,4,1,2,5,2,12,16,6,13,9,13,2,1,1,7,16,4,7,1,19,1,5,1,2,2,7,7,8,2,6,5,4,9,18,7,4,5,9,13,11,8,15,2,1,1,1,2,1,2,2,1,2,2,8,
		2,9,3,3,1,1,4,4,1,1,1,4,9,1,4,3,5,5,2,7,5,3,4,8,2,1,13,2,3,3,1,14,1,1,4,5,1,3,6,1,5,2,1,1,3,3,3,3,1,1,2,7,6,6,7,1,4,7,6,1,1,1,1,1,12,3,3,9,5,
		2,6,1,5,6,1,2,3,18,2,4,14,4,1,3,6,1,1,6,3,5,5,3,2,2,2,2,12,3,1,4,2,3,2,3,11,1,7,4,1,2,1,3,17,1,9,1,24,1,1,4,2,2,4,1,2,7,1,1,1,3,1,2,2,4,15,1,
		1,2,1,1,2,1,5,2,5,20,2,5,9,1,10,8,7,6,1,1,1,1,1,1,6,2,1,2,8,1,1,1,1,5,1,1,3,1,1,1,1,3,1,1,12,4,1,3,1,1,1,1,1,10,3,1,7,5,13,1,2,3,4,6,1,1,30,
		2,9,9,1,15,38,11,3,1,8,24,7,1,9,8,10,2,1,9,31,2,13,6,2,9,4,49,5,2,15,2,1,10,2,1,1,1,2,2,6,15,30,35,3,14,18,8,1,16,10,28,12,19,45,38,1,3,2,3,
		13,2,1,7,3,6,5,3,4,3,1,5,7,8,1,5,3,18,5,3,6,1,21,4,24,9,24,40,3,14,3,21,3,2,1,2,4,2,3,1,15,15,6,5,1,1,3,1,5,6,1,9,7,3,3,2,1,4,3,8,21,5,16,4,
		5,2,10,11,11,3,6,3,2,9,3,6,13,1,2,1,1,1,1,11,12,6,6,1,4,2,6,5,2,1,1,3,3,6,13,3,1,1,5,1,2,3,3,14,2,1,2,2,2,5,1,9,5,1,1,6,12,3,12,3,4,13,2,14,
		2,8,1,17,5,1,16,4,2,2,21,8,9,6,23,20,12,25,19,9,38,8,3,21,40,25,33,13,4,3,1,4,1,2,4,1,2,5,26,2,1,1,2,1,3,6,2,1,1,1,1,1,1,2,3,1,1,1,9,2,3,1,1,
		1,3,6,3,2,1,1,6,6,1,8,2,2,2,1,4,1,2,3,2,7,3,2,4,1,2,1,2,2,1,1,1,1,1,3,1,2,5,4,10,9,4,9,1,1,1,1,1,1,5,3,2,1,6,4,9,6,1,10,2,31,17,8,3,7,5,40,1,
		7,7,1,6,5,2,10,7,8,4,15,39,25,6,28,47,18,10,7,1,3,1,1,2,1,1,1,3,3,3,1,1,1,3,4,2,1,4,1,3,6,10,7,8,6,2,2,1,3,3,2,5,8,7,9,12,2,15,1,1,4,1,2,1,1,
		1,3,2,1,3,3,5,6,2,3,2,10,1,4,2,8,1,1,1,11,6,1,21,4,16,3,1,3,1,4,2,3,6,5,1,3,1,1,3,3,4,6,1,1,10,4,2,7,10,4,7,4,2,9,4,3,1,1,1,4,1,8,3,4,1,3,1,
		6,1,4,2,1,4,7,2,1,8,1,4,5,1,1,2,2,4,6,2,7,1,10,1,1,3,4,11,10,8,21,4,6,1,3,5,2,1,2,28,5,5,2,3,13,1,2,3,1,4,2,1,5,20,3,8,11,1,3,3,3,1,8,10,9,2,
		10,9,2,3,1,1,2,4,1,8,3,6,1,7,8,6,11,1,4,29,8,4,3,1,2,7,13,1,4,1,6,2,6,12,12,2,20,3,2,3,6,4,8,9,2,7,34,5,1,18,6,1,1,4,4,5,7,9,1,2,2,4,3,4,1,7,
		2,2,2,6,2,3,25,5,3,6,1,4,6,7,4,2,1,4,2,13,6,4,4,3,1,5,3,4,4,3,2,1,1,4,1,2,1,1,3,1,11,1,6,3,1,7,3,6,2,8,8,6,9,3,4,11,3,2,10,12,2,5,11,1,6,4,5,
		3,1,8,5,4,6,6,3,5,1,1,3,2,1,2,2,6,17,12,1,10,1,6,12,1,6,6,19,9,6,16,1,13,4,4,15,7,17,6,11,9,15,12,6,7,2,1,2,2,15,9,3,21,4,6,49,18,7,3,2,3,1,
		6,8,2,2,6,2,9,1,3,6,4,4,1,2,16,2,5,2,1,6,2,3,5,3,1,2,5,1,2,1,9,3,1,8,6,4,8,11,3,1,1,1,1,3,1,13,8,4,1,3,2,2,1,4,1,11,1,5,2,1,5,2,5,8,6,1,1,7,
		4,3,8,3,2,7,2,1,5,1,5,2,4,7,6,2,8,5,1,11,4,5,3,6,18,1,2,13,3,3,1,21,1,1,4,1,4,1,1,1,8,1,2,2,7,1,2,4,2,2,9,2,1,1,1,4,3,6,3,12,5,1,1,1,5,6,3,2,
		4,8,2,2,4,2,7,1,8,9,5,2,3,2,1,3,2,13,7,14,6,5,1,1,2,1,4,2,23,2,1,1,6,3,1,4,1,15,3,1,7,3,9,14,1,3,1,4,1,1,5,8,1,3,8,3,8,15,11,4,14,4,4,2,5,5,
		1,7,1,6,14,7,7,8,5,15,4,8,6,5,6,2,1,13,1,20,15,11,9,2,5,6,2,11,2,6,2,5,1,5,8,4,13,19,25,4,1,1,11,1,34,2,5,9,14,6,2,2,6,1,1,14,1,3,14,13,1,6,
		12,21,14,14,6,32,17,8,32,9,28,1,2,4,11,8,3,1,14,2,5,15,1,1,1,1,3,6,4,1,3,4,11,3,1,1,11,30,1,5,1,4,1,5,8,1,1,3,2,4,3,17,35,2,6,12,17,3,1,6,2,
		1,1,12,2,7,3,3,2,1,16,2,8,3,6,5,4,7,3,3,8,1,9,8,5,1,2,1,3,2,8,1,2,9,12,1,1,2,3,8,3,24,12,4,3,7,5,8,3,3,3,3,3,3,1,23,10,3,1,2,2,6,3,1,16,1,16,
		22,3,10,4,11,6,9,7,7,3,6,2,2,2,4,10,2,1,1,2,8,7,1,6,4,1,3,3,3,5,10,12,12,2,3,12,8,15,1,1,16,6,6,1,5,9,11,4,11,4,2,6,12,1,17,5,13,1,4,9,5,1,11,
		2,1,8,1,5,7,28,8,3,5,10,2,17,3,38,22,1,2,18,12,10,4,38,18,1,4,44,19,4,1,8,4,1,12,1,4,31,12,1,14,7,75,7,5,10,6,6,13,3,2,11,11,3,2,5,28,15,6,18,
		18,5,6,4,3,16,1,7,18,7,36,3,5,3,1,7,1,9,1,10,7,2,4,2,6,2,9,7,4,3,32,12,3,7,10,2,23,16,3,1,12,3,31,4,11,1,3,8,9,5,1,30,15,6,12,3,2,2,11,19,9,
		14,2,6,2,3,19,13,17,5,3,3,25,3,14,1,1,1,36,1,3,2,19,3,13,36,9,13,31,6,4,16,34,2,5,4,2,3,3,5,1,1,1,4,3,1,17,3,2,3,5,3,1,3,2,3,5,6,3,12,11,1,3,
		1,2,26,7,12,7,2,14,3,3,7,7,11,25,25,28,16,4,36,1,2,1,6,2,1,9,3,27,17,4,3,4,13,4,1,3,2,2,1,10,4,2,4,6,3,8,2,1,18,1,1,24,2,2,4,33,2,3,63,7,1,6,
		40,7,3,4,4,2,4,15,18,1,16,1,1,11,2,41,14,1,3,18,13,3,2,4,16,2,17,7,15,24,7,18,13,44,2,2,3,6,1,1,7,5,1,7,1,4,3,3,5,10,8,2,3,1,8,1,1,27,4,2,1,
		12,1,2,1,10,6,1,6,7,5,2,3,7,11,5,11,3,6,6,2,3,15,4,9,1,1,2,1,2,11,2,8,12,8,5,4,2,3,1,5,2,2,1,14,1,12,11,4,1,11,17,17,4,3,2,5,5,7,3,1,5,9,9,8,
		2,5,6,6,13,13,2,1,2,6,1,2,2,49,4,9,1,2,10,16,7,8,4,3,2,23,4,58,3,29,1,14,19,19,11,11,2,7,5,1,3,4,6,2,18,5,12,12,17,17,3,3,2,4,1,6,2,3,4,3,1,
		1,1,1,5,1,1,9,1,3,1,3,6,1,8,1,1,2,6,4,14,3,1,4,11,4,1,3,32,1,2,4,13,4,1,2,4,2,1,3,1,11,1,4,2,1,4,4,6,3,5,1,6,5,7,6,3,23,3,5,3,5,3,3,13,3,9,10,
		1,12,10,2,3,18,13,7,160,52,4,2,2,3,2,14,5,4,12,4,6,4,1,20,4,11,6,2,12,27,1,4,1,2,2,7,4,5,2,28,3,7,25,8,3,19,3,6,10,2,2,1,10,2,5,4,1,3,4,1,5,
		3,2,6,9,3,6,2,16,3,3,16,4,5,5,3,2,1,2,16,15,8,2,6,21,2,4,1,22,5,8,1,1,21,11,2,1,11,11,19,13,12,4,2,3,2,3,6,1,8,11,1,4,2,9,5,2,1,11,2,9,1,1,2,
		14,31,9,3,4,21,14,4,8,1,7,2,2,2,5,1,4,20,3,3,4,10,1,11,9,8,2,1,4,5,14,12,14,2,17,9,6,31,4,14,1,20,13,26,5,2,7,3,6,13,2,4,2,19,6,2,2,18,9,3,5,
		12,12,14,4,6,2,3,6,9,5,22,4,5,25,6,4,8,5,2,6,27,2,35,2,16,3,7,8,8,6,6,5,9,17,2,20,6,19,2,13,3,1,1,1,4,17,12,2,14,7,1,4,18,12,38,33,2,10,1,1,
		2,13,14,17,11,50,6,33,20,26,74,16,23,45,50,13,38,33,6,6,7,4,4,2,1,3,2,5,8,7,8,9,3,11,21,9,13,1,3,10,6,7,1,2,2,18,5,5,1,9,9,2,68,9,19,13,2,5,
		1,4,4,7,4,13,3,9,10,21,17,3,26,2,1,5,2,4,5,4,1,7,4,7,3,4,2,1,6,1,1,20,4,1,9,2,2,1,3,3,2,3,2,1,1,1,20,2,3,1,6,2,3,6,2,4,8,1,3,2,10,3,5,3,4,4,
		3,4,16,1,6,1,10,2,4,2,1,1,2,10,11,2,2,3,1,24,31,4,10,10,2,5,12,16,164,15,4,16,7,9,15,19,17,1,2,1,1,5,1,1,1,1,1,3,1,4,3,1,3,1,3,1,2,1,1,3,3,7,
		2,8,1,2,2,2,1,3,4,3,7,8,12,92,2,10,3,1,3,14,5,25,16,42,4,7,7,4,2,21,5,27,26,27,21,25,30,31,2,1,5,13,3,22,5,6,6,11,9,12,1,5,9,7,5,5,22,60,3,5,
		13,1,1,8,1,1,3,3,2,1,9,3,3,18,4,1,2,3,7,6,3,1,2,3,9,1,3,1,3,2,1,3,1,1,1,2,1,11,3,1,6,9,1,3,2,3,1,2,1,5,1,1,4,3,4,1,2,2,4,4,1,7,2,1,2,2,3,5,13,
		18,3,4,14,9,9,4,16,3,7,5,8,2,6,48,28,3,1,1,4,2,14,8,2,9,2,1,15,2,4,3,2,10,16,12,8,7,1,1,3,1,1,1,2,7,4,1,6,4,38,39,16,23,7,15,15,3,2,12,7,21,
		37,27,6,5,4,8,2,10,8,8,6,5,1,2,1,3,24,1,16,17,9,23,10,17,6,1,51,55,44,13,294,9,3,6,2,4,2,2,15,1,1,1,13,21,17,68,14,8,9,4,1,4,9,3,11,7,1,1,1,
		5,6,3,2,1,1,1,2,3,8,1,2,2,4,1,5,5,2,1,4,3,7,13,4,1,4,1,3,1,1,1,5,5,10,1,6,1,5,2,1,5,2,4,1,4,5,7,3,18,2,9,11,32,4,3,3,2,4,7,11,16,9,11,8,13,38,
		32,8,4,2,1,1,2,1,2,4,4,1,1,1,4,1,21,3,11,1,16,1,1,6,1,3,2,4,9,8,57,7,44,1,3,3,13,3,10,1,1,7,5,2,7,21,47,63,3,15,4,7,1,16,1,1,2,8,2,3,42,15,4,
		1,29,7,22,10,3,78,16,12,20,18,4,67,11,5,1,3,15,6,21,31,32,27,18,13,71,35,5,142,4,10,1,2,50,19,33,16,35,37,16,19,27,7,1,133,19,1,4,8,7,20,1,4,
		4,1,10,3,1,6,1,2,51,5,40,15,24,43,22928,11,1,13,154,70,3,1,1,7,4,10,1,2,1,1,2,1,2,1,2,2,1,1,2,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,
		3,2,1,1,1,1,2,1,1,
	};
	static XyWchar base_ranges[] = // not zero-terminated
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
		0x31F0, 0x31FF, // Katakana Phonetic Extensions
		0xFF00, 0xFFEF, // Half-width characters
		0xFFFD, 0xFFFD  // Invalid
	};
	static XyWchar full_ranges[XY_ARRAYSIZE(base_ranges) + XY_ARRAYSIZE(accumulative_offsets_from_0x4E00) * 2 + 1] = { 0 };
	if (!full_ranges[0])
	{
		memcpy(full_ranges, base_ranges, sizeof(base_ranges));
		UnpackAccumulativeOffsetsIntoRanges(0x4E00, accumulative_offsets_from_0x4E00, XY_ARRAYSIZE(accumulative_offsets_from_0x4E00), full_ranges + XY_ARRAYSIZE(base_ranges));
	}
	return &full_ranges[0];
}

const XyWchar* XyFontAtlas::GetGlyphRangesCyrillic()
{
	static const XyWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0,
	};
	return &ranges[0];
}

const XyWchar* XyFontAtlas::GetGlyphRangesThai()
{
	static const XyWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin
		0x2010, 0x205E, // Punctuations
		0x0E00, 0x0E7F, // Thai
		0,
	};
	return &ranges[0];
}

const XyWchar* XyFontAtlas::GetGlyphRangesVietnamese()
{
	static const XyWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin
		0x0102, 0x0103,
		0x0110, 0x0111,
		0x0128, 0x0129,
		0x0168, 0x0169,
		0x01A0, 0x01A1,
		0x01AF, 0x01B0,
		0x1EA0, 0x1EF9,
		0,
	};
	return &ranges[0];
}


const XyWchar* XyFontAtlas::GetGlyphRangesChineseSimplifiedCommon()
{
	// Store 2500 regularly used characters for Simplified Chinese.
	// Sourced from https://zh.wiktionary.org/wiki/%E9%99%84%E5%BD%95:%E7%8E%B0%E4%BB%A3%E6%B1%89%E8%AF%AD%E5%B8%B8%E7%94%A8%E5%AD%97%E8%A1%A8
	// This table covers 97.97% of all characters used during the month in July, 1987.
	// You can use ImFontGlyphRangesBuilder to create your own ranges derived from this, by merging existing ranges or adding new characters.
	// (Stored as accumulative offsets from the initial unicode codepoint 0x4E00. This encoding is designed to helps us compact the source code size.)
	static const short accumulative_offsets_from_0x4E00[] =
	{
		0,1,2,4,1,1,1,1,2,1,3,2,1,2,2,1,1,1,1,1,5,2,1,2,3,3,3,2,2,4,1,1,1,2,1,5,2,3,1,2,1,2,1,1,2,1,1,2,2,1,4,1,1,1,1,5,10,1,2,19,2,1,2,1,2,1,2,1,2,
		1,5,1,6,3,2,1,2,2,1,1,1,4,8,5,1,1,4,1,1,3,1,2,1,5,1,2,1,1,1,10,1,1,5,2,4,6,1,4,2,2,2,12,2,1,1,6,1,1,1,4,1,1,4,6,5,1,4,2,2,4,10,7,1,1,4,2,4,
		2,1,4,3,6,10,12,5,7,2,14,2,9,1,1,6,7,10,4,7,13,1,5,4,8,4,1,1,2,28,5,6,1,1,5,2,5,20,2,2,9,8,11,2,9,17,1,8,6,8,27,4,6,9,20,11,27,6,68,2,2,1,1,
		1,2,1,2,2,7,6,11,3,3,1,1,3,1,2,1,1,1,1,1,3,1,1,8,3,4,1,5,7,2,1,4,4,8,4,2,1,2,1,1,4,5,6,3,6,2,12,3,1,3,9,2,4,3,4,1,5,3,3,1,3,7,1,5,1,1,1,1,2,
		3,4,5,2,3,2,6,1,1,2,1,7,1,7,3,4,5,15,2,2,1,5,3,22,19,2,1,1,1,1,2,5,1,1,1,6,1,1,12,8,2,9,18,22,4,1,1,5,1,16,1,2,7,10,15,1,1,6,2,4,1,2,4,1,6,
		1,1,3,2,4,1,6,4,5,1,2,1,1,2,1,10,3,1,3,2,1,9,3,2,5,7,2,19,4,3,6,1,1,1,1,1,4,3,2,1,1,1,2,5,3,1,1,1,2,2,1,1,2,1,1,2,1,3,1,1,1,3,7,1,4,1,1,2,1,
		1,2,1,2,4,4,3,8,1,1,1,2,1,3,5,1,3,1,3,4,6,2,2,14,4,6,6,11,9,1,15,3,1,28,5,2,5,5,3,1,3,4,5,4,6,14,3,2,3,5,21,2,7,20,10,1,2,19,2,4,28,28,2,3,
		2,1,14,4,1,26,28,42,12,40,3,52,79,5,14,17,3,2,2,11,3,4,6,3,1,8,2,23,4,5,8,10,4,2,7,3,5,1,1,6,3,1,2,2,2,5,28,1,1,7,7,20,5,3,29,3,17,26,1,8,4,
		27,3,6,11,23,5,3,4,6,13,24,16,6,5,10,25,35,7,3,2,3,3,14,3,6,2,6,1,4,2,3,8,2,1,1,3,3,3,4,1,1,13,2,2,4,5,2,1,14,14,1,2,2,1,4,5,2,3,1,14,3,12,
		3,17,2,16,5,1,2,1,8,9,3,19,4,2,2,4,17,25,21,20,28,75,1,10,29,103,4,1,2,1,1,4,2,4,1,2,3,24,2,2,2,1,1,2,1,3,8,1,1,1,2,1,1,3,1,1,1,6,1,5,3,1,1,
		1,3,4,1,1,5,2,1,5,6,13,9,16,1,1,1,1,3,2,3,2,4,5,2,5,2,2,3,7,13,7,2,2,1,1,1,1,2,3,3,2,1,6,4,9,2,1,14,2,14,2,1,18,3,4,14,4,11,41,15,23,15,23,
		176,1,3,4,1,1,1,1,5,3,1,2,3,7,3,1,1,2,1,2,4,4,6,2,4,1,9,7,1,10,5,8,16,29,1,1,2,2,3,1,3,5,2,4,5,4,1,1,2,2,3,3,7,1,6,10,1,17,1,44,4,6,2,1,1,6,
		5,4,2,10,1,6,9,2,8,1,24,1,2,13,7,8,8,2,1,4,1,3,1,3,3,5,2,5,10,9,4,9,12,2,1,6,1,10,1,1,7,7,4,10,8,3,1,13,4,3,1,6,1,3,5,2,1,2,17,16,5,2,16,6,
		1,4,2,1,3,3,6,8,5,11,11,1,3,3,2,4,6,10,9,5,7,4,7,4,7,1,1,4,2,1,3,6,8,7,1,6,11,5,5,3,24,9,4,2,7,13,5,1,8,82,16,61,1,1,1,4,2,2,16,10,3,8,1,1,
		6,4,2,1,3,1,1,1,4,3,8,4,2,2,1,1,1,1,1,6,3,5,1,1,4,6,9,2,1,1,1,2,1,7,2,1,6,1,5,4,4,3,1,8,1,3,3,1,3,2,2,2,2,3,1,6,1,2,1,2,1,3,7,1,8,2,1,2,1,5,
		2,5,3,5,10,1,2,1,1,3,2,5,11,3,9,3,5,1,1,5,9,1,2,1,5,7,9,9,8,1,3,3,3,6,8,2,3,2,1,1,32,6,1,2,15,9,3,7,13,1,3,10,13,2,14,1,13,10,2,1,3,10,4,15,
		2,15,15,10,1,3,9,6,9,32,25,26,47,7,3,2,3,1,6,3,4,3,2,8,5,4,1,9,4,2,2,19,10,6,2,3,8,1,2,2,4,2,1,9,4,4,4,6,4,8,9,2,3,1,1,1,1,3,5,5,1,3,8,4,6,
		2,1,4,12,1,5,3,7,13,2,5,8,1,6,1,2,5,14,6,1,5,2,4,8,15,5,1,23,6,62,2,10,1,1,8,1,2,2,10,4,2,2,9,2,1,1,3,2,3,1,5,3,3,2,1,3,8,1,1,1,11,3,1,1,4,
		3,7,1,14,1,2,3,12,5,2,5,1,6,7,5,7,14,11,1,3,1,8,9,12,2,1,11,8,4,4,2,6,10,9,13,1,1,3,1,5,1,3,2,4,4,1,18,2,3,14,11,4,29,4,2,7,1,3,13,9,2,2,5,
		3,5,20,7,16,8,5,72,34,6,4,22,12,12,28,45,36,9,7,39,9,191,1,1,1,4,11,8,4,9,2,3,22,1,1,1,1,4,17,1,7,7,1,11,31,10,2,4,8,2,3,2,1,4,2,16,4,32,2,
		3,19,13,4,9,1,5,2,14,8,1,1,3,6,19,6,5,1,16,6,2,10,8,5,1,2,3,1,5,5,1,11,6,6,1,3,3,2,6,3,8,1,1,4,10,7,5,7,7,5,8,9,2,1,3,4,1,1,3,1,3,3,2,6,16,
		1,4,6,3,1,10,6,1,3,15,2,9,2,10,25,13,9,16,6,2,2,10,11,4,3,9,1,2,6,6,5,4,30,40,1,10,7,12,14,33,6,3,6,7,3,1,3,1,11,14,4,9,5,12,11,49,18,51,31,
		140,31,2,2,1,5,1,8,1,10,1,4,4,3,24,1,10,1,3,6,6,16,3,4,5,2,1,4,2,57,10,6,22,2,22,3,7,22,6,10,11,36,18,16,33,36,2,5,5,1,1,1,4,10,1,4,13,2,7,
		5,2,9,3,4,1,7,43,3,7,3,9,14,7,9,1,11,1,1,3,7,4,18,13,1,14,1,3,6,10,73,2,2,30,6,1,11,18,19,13,22,3,46,42,37,89,7,3,16,34,2,2,3,9,1,7,1,1,1,2,
		2,4,10,7,3,10,3,9,5,28,9,2,6,13,7,3,1,3,10,2,7,2,11,3,6,21,54,85,2,1,4,2,2,1,39,3,21,2,2,5,1,1,1,4,1,1,3,4,15,1,3,2,4,4,2,3,8,2,20,1,8,7,13,
		4,1,26,6,2,9,34,4,21,52,10,4,4,1,5,12,2,11,1,7,2,30,12,44,2,30,1,1,3,6,16,9,17,39,82,2,2,24,7,1,7,3,16,9,14,44,2,1,2,1,2,3,5,2,4,1,6,7,5,3,
		2,6,1,11,5,11,2,1,18,19,8,1,3,24,29,2,1,3,5,2,2,1,13,6,5,1,46,11,3,5,1,1,5,8,2,10,6,12,6,3,7,11,2,4,16,13,2,5,1,1,2,2,5,2,28,5,2,23,10,8,4,
		4,22,39,95,38,8,14,9,5,1,13,5,4,3,13,12,11,1,9,1,27,37,2,5,4,4,63,211,95,2,2,2,1,3,5,2,1,1,2,2,1,1,1,3,2,4,1,2,1,1,5,2,2,1,1,2,3,1,3,1,1,1,
		3,1,4,2,1,3,6,1,1,3,7,15,5,3,2,5,3,9,11,4,2,22,1,6,3,8,7,1,4,28,4,16,3,3,25,4,4,27,27,1,4,1,2,2,7,1,3,5,2,28,8,2,14,1,8,6,16,25,3,3,3,14,3,
		3,1,1,2,1,4,6,3,8,4,1,1,1,2,3,6,10,6,2,3,18,3,2,5,5,4,3,1,5,2,5,4,23,7,6,12,6,4,17,11,9,5,1,1,10,5,12,1,1,11,26,33,7,3,6,1,17,7,1,5,12,1,11,
		2,4,1,8,14,17,23,1,2,1,7,8,16,11,9,6,5,2,6,4,16,2,8,14,1,11,8,9,1,1,1,9,25,4,11,19,7,2,15,2,12,8,52,7,5,19,2,16,4,36,8,1,16,8,24,26,4,6,2,9,
		5,4,36,3,28,12,25,15,37,27,17,12,59,38,5,32,127,1,2,9,17,14,4,1,2,1,1,8,11,50,4,14,2,19,16,4,17,5,4,5,26,12,45,2,23,45,104,30,12,8,3,10,2,2,
		3,3,1,4,20,7,2,9,6,15,2,20,1,3,16,4,11,15,6,134,2,5,59,1,2,2,2,1,9,17,3,26,137,10,211,59,1,2,4,1,4,1,1,1,2,6,2,3,1,1,2,3,2,3,1,3,4,4,2,3,3,
		1,4,3,1,7,2,2,3,1,2,1,3,3,3,2,2,3,2,1,3,14,6,1,3,2,9,6,15,27,9,34,145,1,1,2,1,1,1,1,2,1,1,1,1,2,2,2,3,1,2,1,1,1,2,3,5,8,3,5,2,4,1,3,2,2,2,12,
		4,1,1,1,10,4,5,1,20,4,16,1,15,9,5,12,2,9,2,5,4,2,26,19,7,1,26,4,30,12,15,42,1,6,8,172,1,1,4,2,1,1,11,2,2,4,2,1,2,1,10,8,1,2,1,4,5,1,2,5,1,8,
		4,1,3,4,2,1,6,2,1,3,4,1,2,1,1,1,1,12,5,7,2,4,3,1,1,1,3,3,6,1,2,2,3,3,3,2,1,2,12,14,11,6,6,4,12,2,8,1,7,10,1,35,7,4,13,15,4,3,23,21,28,52,5,
		26,5,6,1,7,10,2,7,53,3,2,1,1,1,2,163,532,1,10,11,1,3,3,4,8,2,8,6,2,2,23,22,4,2,2,4,2,1,3,1,3,3,5,9,8,2,1,2,8,1,10,2,12,21,20,15,105,2,3,1,1,
		3,2,3,1,1,2,5,1,4,15,11,19,1,1,1,1,5,4,5,1,1,2,5,3,5,12,1,2,5,1,11,1,1,15,9,1,4,5,3,26,8,2,1,3,1,1,15,19,2,12,1,2,5,2,7,2,19,2,20,6,26,7,5,
		2,2,7,34,21,13,70,2,128,1,1,2,1,1,2,1,1,3,2,2,2,15,1,4,1,3,4,42,10,6,1,49,85,8,1,2,1,1,4,4,2,3,6,1,5,7,4,3,211,4,1,2,1,2,5,1,2,4,2,2,6,5,6,
		10,3,4,48,100,6,2,16,296,5,27,387,2,2,3,7,16,8,5,38,15,39,21,9,10,3,7,59,13,27,21,47,5,21,6
	};
	static XyWchar base_ranges[] = // not zero-terminated
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x2000, 0x206F, // General Punctuation
		0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
		0x31F0, 0x31FF, // Katakana Phonetic Extensions
		0xFF00, 0xFFEF, // Half-width characters
		0xFFFD, 0xFFFD  // Invalid
	};
	static XyWchar full_ranges[XY_ARRAYSIZE(base_ranges) + XY_ARRAYSIZE(accumulative_offsets_from_0x4E00) * 2 + 1] = { 0 };
	if (!full_ranges[0])
	{
		memcpy(full_ranges, base_ranges, sizeof(base_ranges));
		UnpackAccumulativeOffsetsIntoRanges(0x4E00, accumulative_offsets_from_0x4E00, XY_ARRAYSIZE(accumulative_offsets_from_0x4E00), full_ranges + XY_ARRAYSIZE(base_ranges));
	}
	return &full_ranges[0];
}

void XyFontGlyphRangesBuilder::AddText(const char* text, const char* text_end)
{
	while (text_end ? (text < text_end) : *text)
	{
		unsigned int c = 0;
		int c_len = XyTextCharFromUtf8(&c, text, text_end);
		text += c_len;
		if (c_len == 0)
			break;
		AddChar((XyWchar)c);
	}
}

void XyFontGlyphRangesBuilder::AddRanges(const XyWchar* ranges)
{
	for (; ranges[0]; ranges += 2)
		for (unsigned int c = ranges[0]; c <= ranges[1] && c <= XY_UNICODE_CODEPOINT_MAX; c++) //-V560
			AddChar((XyWchar)c);
}

void XyFontGlyphRangesBuilder::BuildRanges(XyVector<XyWchar>* out_ranges)
{
	const int max_codepoint = XY_UNICODE_CODEPOINT_MAX;
	for (int n = 0; n <= max_codepoint; n++)
		if (GetBit(n))
		{
			out_ranges->push_back((XyWchar)n);
			while (n < max_codepoint && GetBit(n + 1))
				n++;
			out_ranges->push_back((XyWchar)n);
		}
	out_ranges->push_back(0);
}
