10 close 2: open 2,2,0,chr$(6) + chr$(0)
15 poke 53280, 0: poke 53281, 0
20 blank$="                           "
25 print "{wht}"

100 x=0: y=0: gosub 1000
105 print "{clr}{rvon}  commodore 64 B 300bps B 8n1 B noctrl  {rvof}";
110 x=2: y=2: gosub 1000
115 print chr$(176) + "{rvon}  sensor dht22  {rvof}" + chr$(174)
120 x=20: y=2: gosub 1000
125 print chr$(176) + "{rvon}sensor mpl3115a2{rvof}" + chr$(174)
130 for i=0 to 8
135 x=2: y=i+3: gosub 1000: print chr$(98)
140 x=19: y=i+3: gosub 1000: print chr$(98)
145 x=20: y=i+3: gosub 1000: print chr$(98)
150 x=37: y=i+3: gosub 1000: print chr$(98)
155 next i
160 x=2: y=12: gosub 1000
165 print chr$(171) + "{rvon} sensor nova pm {rvof}" + chr$(179)
170 x=20: y=12: gosub 1000
175 print chr$(171);: for i=1 to 16: print chr$(99);: next i: print chr$(179);
180 for i=0 to 6
185 x=2: y=i+13: gosub 1000: print chr$(98)
190 x=19: y=i+13: gosub 1000: print chr$(98)
195 x=20: y=i+13: gosub 1000: print chr$(98)
200 x=37: y=i+13: gosub 1000: print chr$(98)
205 next i
210 x=2: y=20: gosub 1000
215 print chr$(171);
220 for i=1 to 16: print chr$(99);: next i
225 print chr$(177);: print chr$(177);
230 for i=1 to 16: print chr$(99);: next i
235 print chr$(179);
240 x=2: y=21: gosub 1000: print chr$(98);
245 x=37: y=21: gosub 1000: print chr$(98);
250 x=2: y=22: gosub 1000
255 print chr$(173);: for i=1 to 34: print chr$(99);: next i: print chr$(189)

260 x=5:y=4: gosub 1000: print "temp:      c";
265 x=5:y=5: gosub 1000: print "hume:      %";
270 x=5:y=6: gosub 1000: print "volt:      v";

275 x=22:y=4: gosub 1000: print "temp:       c";
280 x=22:y=5: gosub 1000: print "pres:       h";
285 x=22:y=6: gosub 1000: print "alti:       m";
290 x=22:y=7: gosub 1000: print "volt:       v";

295 x=4:y=14: gosub 1000: print "pm2.5:       u";
300 x=4:y=15: gosub 1000: print "pm10 :       u";
305 x=4:y=16: gosub 1000: print "volt :       v";

310 x=22:y=14: gosub 1000: print "xxx      xxxxx";
315 x=22:y=15: gosub 1000: print " x   xx    x  ";
320 x=22:y=16: gosub 1000: print " x  x  x   x  ";
325 x=22:y=17: gosub 1000: print " x  x  x   x  ";
330 x=22:y=18: gosub 1000: print "xxx  xx    x  ";

400 gosub 1100: cmmd$="stat": tj=3*60: gosub 2100
405 if resp$="+ready" then goto 450
410 x=10:y=21:gosub 1000: print "respuesta invalida"
415 tj=2*60: gosub 1200
420 goto 400

450 gosub 1100: cmmd$="temp": tj=3*60: gosub 2100
455 if left$(resp$,5) <> "+temp" then goto 500
460 x=11:y=4:gosub 1000: print mid$(resp$,7,5);
465 x=11:y=5:gosub 1000: print mid$(resp$,13,5);
470 x=11:y=6:gosub 1000: print mid$(resp$,19,5);
475 x=6:y=9:gosub 1000: print mid$(resp$,25,10);
480 x=7:y=10:gosub 1000: print mid$(resp$,36,8);

500 gosub 1100: cmmd$="pres": tj=3*60: gosub 2100
505 if left$(resp$,5) <> "+pres" then goto 550
510 x=28:y=4:gosub 1000: print mid$(resp$,7,6);
515 x=28:y=5:gosub 1000: print mid$(resp$,14,6);
520 x=28:y=6:gosub 1000: print mid$(resp$,21,6);
525 x=28:y=7:gosub 1000: print mid$(resp$,28,6);
530 x=23:y=9:gosub 1000: print mid$(resp$,35,10);
535 x=24:y=10:gosub 1000: print mid$(resp$,46,8);

550 gosub 1100: cmmd$="nova": tj=3*60: gosub 2100
555 if left$(resp$,5) <> "+nova" then goto 900
560 x=11:y=14:gosub 1000: print mid$(resp$,7,6);
565 x=11:y=15:gosub 1000: print mid$(resp$,14,6);
570 x=11:y=16:gosub 1000: print mid$(resp$,21,6);
575 x=6:y=17:gosub 1000: print mid$(resp$,28,10);
580 x=7:y=18:gosub 1000: print mid$(resp$,39,8);

900 x=4:y=21:gosub 1000: print "     "; blank$
905 tj=10*60: gosub 1200
910 goto 400

1000 rem goto x,y
1005 poke 781, y
1010 poke 782, x
1015 poke 783, 0
1020 sys 65520
1025 return

1100 rem revisa el teclado
1105 get k$
1110 if k$="" then return
1115 if k$ <> chr$(3) then return
1120 close 2
1125 print "{clr}"
1130 end

1200 rem genera una pausa de tj jiffies
1205 t=ti + tj
1210 if ti > t then return
1215 gosub 1100
1220 goto 1210

2000 rem lee una linea desde el modem
2005 resp$=""
2010 t = ti + tj
2015 get#2, ch$
2020 if ch$<>"" then goto 2035
2025 if ti>t then resp$="": return
2030 goto 2015
2035 if ch$ = chr$(13) then return
2040 if asc(ch$)<32 or asc(ch$)>124 then ch$="?"
2045 resp$ = resp$ + ch$
2050 goto 2010

2100 rem interacua con el broker
2105 ttjj=tj: tj=30
2110 gosub 2000: if resp$<>"" then goto 2110
2115 x=4:y=21:gosub 1000: print cmmd$; ": "; blank$;
2120 print#2,cmmd$;
2125 print#2,chr$(13);
2130 tj=ttjj: gosub 2000
2135 x=10: y=21: gosub 1000
2140 print left$(resp$,26);
2145 return

