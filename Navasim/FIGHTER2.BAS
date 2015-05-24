   DECLARE SUB FLINE (p1%, p2%, p3%, p4%)
   DECLARE SUB FRLINE (p1%, p2%)
   DECLARE SUB draw.boat1 (c%)
   DECLARE SUB draw.boat2 ()

   'load line drawing primitive

   DIM space%(350)
   DIM SHARED fseg%, flin%, xl%, yl%
   fseg% = VARSEG(space%(0))
   flin% = VARPTR(space%(0))
   IF flin% <> 0 THEN BEEP: END
   flin% = &H100
   fseg% = fseg% - &H10
   OPEN "C:\EASYASM\FASTLINE.COM" FOR INPUT AS 1
   DEF SEG = fseg%
   FOR a% = 256 TO 256 + 643
      w% = ASC(INPUT$(1, 1))
      POKE a%, w%
   NEXT
  
   'load the screen clear routine.

   DIM SHARED clr%(0 TO 12)
   DIM SHARED clseg%, clp%
   clseg% = VARSEG(clr%(0))
   clp% = VARPTR(clr%(0))
   DEF SEG = clseg%
   RESTORE clear.data
   FOR a% = 0 TO 24
      READ c$: c% = VAL("&H" + c$)
      POKE p% + a%, c%
   NEXT

   'load the screen move routine.

   DIM SHARED move%(0 TO 31)
   DIM SHARED mseg%, mp%
   mseg% = VARSEG(move%(0))
   mp% = VARPTR(move%(0))
   DIM SHARED buffer%(0 TO 14000)
   DIM SHARED buf%
   buf% = VARSEG(buffer%(0))
   IF VARPTR(buffer%(0)) <> 0 THEN BEEP: END
   DEF SEG = mseg%
   RESTORE move.data
   FOR a% = 0 TO 31
      READ c$: c% = VAL("&H" + c$)
      POKE p% + a%, c%
   NEXT

   CLS : SCREEN 10: max = 19
   pi = 3.141593: pi2 = pi / 180
   DIM SHARED box%(0 TO max, 0 TO 2)
   DIM SHARED box2%(0 TO max, 0 TO 2)
   xof% = 320: yof% = 175
   'vectors
   xx = 1: xy = 0: xz = 0   ' [xx,xy,xz]=[1,0,0]
   yx = 0: yy = 1: yz = 0   ' [yx,yy,yz]=[0,1,0]
   zx = 0: zy = 0: zz = 1   ' [zx,zy,zz]=[0,0,1]
   b = 2 * TAN(45 * pi2)

clear.data:
   DATA 55, 89,E5, 8B,5E,06, 5D,06, 8E,07, B9,B0,36, BF,00,00, B8,00,00
   DATA F3, AB, 07, CA,02,00

move.data:
   DATA 55, 89,E5, 8B,5E,06, 5D, 1E, 06, B8,00,A0, 8E,C0, BF,00,00, 8E,1F
   DATA BE,00,00, B9,B0,36, F3, A5, 07, 1F, CA,02,00

fighter.data:
   DATA 30,0,30, 30,26,20, 30,26,-20, 30,0,-30, 30,-26,-20, 30,-26,20
   DATA -30,0,30, -30,26,20, -30,26,-20, -30,0,-30, -30,-26,-20, -30,-26,20
   DATA 30,0,0, 10,0,0, 0,10,0, -10,0,0, 0,-10,0, -30,0,0, 0,0,10, 0,0,-10

   RESTORE fighter.data
   FOR a% = 0 TO max
      READ box%(a%, 0), box%(a%, 1), box%(a%, 2)
      box%(a%, 0) = 2 * box%(a%, 0)
      box%(a%, 1) = 2 * box%(a%, 1)
      box%(a%, 2) = 2 * box%(a%, 2)
   NEXT

   xx = 1: xy = 0: xz = 0: yx = 0: yy = 1: yz = 0: zx = 0: zy = 0: zz = 1
   zf = 25 * pi2: cosine = COS(zf): sine = SIN(zf)
   xxt = cosine * xx + sine * yx: xyt = cosine * xy + sine * yy: xzt = cosine * xz + sine * yz
   yx = cosine * yx - sine * xx: yy = cosine * yy - sine * xy: yz = cosine * yz - sine * xz
   xx = xxt: xy = xyt: xz = xzt

   xa = xx: xb = xy: xc = xz
   ya = yx: yb = yy: yc = yz
   za = zx: zb = zy: zc = zz
rep:
   xx = xa: xy = xb: xz = xc: yx = ya: yy = yb: yz = yc: zx = za: zy = zb: zz = zc

   yf = r% * pi2: cosine = COS(yf): sine = SIN(yf)
   xxt = cosine * xx + sine * zx: xyt = cosine * xy + sine * zy: xzt = cosine * xz + sine * zz
   zx = cosine * zx - sine * xx: zy = cosine * zy - sine * xy: zz = cosine * zz - sine * xz
   xx = xxt: xy = xyt: xz = xzt

   'absolute y
   'yf = r% * pi2: cosine = COS(yf): sine = SIN(yf)
   'xxt = cosine * 1 + sine * 0: xyt = cosine * 0 + sine * 0: xzt = cosine * 0 + sine * 1
   'zx = cosine * 0 - sine * 1: zy = cosine * 0 - sine * 0: zz = cosine * 1 - sine * 0
   'xx = xxt: xy = xyt: xz = xzt

   xf = r% * pi2: cosine = COS(xf): sine = SIN(xf)
   zxt = zx * cosine + yx * sine: zyt = zy * cosine + yy * sine: zzt = zz * cosine + yz * sine
   yx = yx * cosine - zx * sine: yy = yy * cosine - zy * sine: yz = yz * cosine - zz * sine
   zx = zxt: zy = zyt: zz = zzt

    r% = (r% + 2) MOD 360
    p = 200 * SIN(r% * pi2) + 350
convert:
   FOR a% = 0 TO max
      b% = box%(a%, 0): c% = box%(a%, 1): d% = box%(a%, 2)
      fov = b * (d% * zz + p)
      box2%(a%, 0) = b% * xx + c% * yx + d% * zx
      box2%(a%, 1) = (b% * xy + c% * yy + d% * zy) * -.75
      box2%(a%, 0) = (640 / fov) * box2%(a%, 0) + xof%
      box2%(a%, 1) = (640 / fov) * box2%(a%, 1) + yof%
   NEXT

'clear the offscreen buffer
   DEF SEG = clseg%
   CALL absolute(buf%, clp%)
   DEF SEG
'draw the boat
   draw.boat2
'move the data from the offscreen buffer into the video buffer
   DEF SEG = mseg%
   CALL absolute(buf%, mp%)
   DEF SEG

   w$ = INKEY$: IF w$ = CHR$(27) THEN END ELSE IF w$ = " " THEN SLEEP 2
   GOTO rep

SUB draw.boat1 (c%)

   LINE (box2%(0, 0), box2%(0, 1))-(box2%(1, 0), box2%(1, 1))
   LINE -(box2%(2, 0), box2%(2, 1))
   LINE -(box2%(3, 0), box2%(3, 1))
   LINE -(box2%(4, 0), box2%(4, 1))
   LINE -(box2%(5, 0), box2%(5, 1))
   LINE -(box2%(0, 0), box2%(0, 1))
   LINE (box2%(6, 0), box2%(6, 1))-(box2%(7, 0), box2%(7, 1))
   LINE -(box2%(8, 0), box2%(8, 1))
   LINE -(box2%(9, 0), box2%(9, 1))
   LINE -(box2%(10, 0), box2%(10, 1))
   LINE -(box2%(11, 0), box2%(11, 1))
   LINE -(box2%(6, 0), box2%(6, 1))
   LINE (box2%(12, 0), box2%(12, 1))-(box2%(13, 0), box2%(13, 1))
   LINE -(box2%(14, 0), box2%(14, 1))
   LINE -(box2%(15, 0), box2%(15, 1))
   LINE -(box2%(16, 0), box2%(16, 1))
   LINE -(box2%(13, 0), box2%(13, 1))
   LINE (box2%(15, 0), box2%(15, 1))-(box2%(17, 0), box2%(17, 1))
   LINE (box2%(14, 0), box2%(14, 1))-(box2%(18, 0), box2%(18, 1))
   LINE -(box2%(16, 0), box2%(16, 1))
   LINE (box2%(14, 0), box2%(14, 1))-(box2%(19, 0), box2%(19, 1))
   LINE -(box2%(16, 0), box2%(16, 1))
   LINE (box2%(13, 0), box2%(13, 1))-(box2%(18, 0), box2%(18, 1))
   LINE -(box2%(15, 0), box2%(15, 1))
   LINE -(box2%(19, 0), box2%(19, 1))
   LINE -(box2%(13, 0), box2%(13, 1))
   LINE (box2%(1, 0), box2%(1, 1))-(box2%(4, 0), box2%(4, 1))
   LINE (box2%(2, 0), box2%(2, 1))-(box2%(5, 0), box2%(5, 1))
   LINE (box2%(7, 0), box2%(7, 1))-(box2%(10, 0), box2%(10, 1))
   LINE (box2%(8, 0), box2%(8, 1))-(box2%(11, 0), box2%(11, 1))

END SUB

SUB draw.boat2

   FLINE box2%(0, 0), box2%(0, 1), box2%(1, 0), box2%(1, 1)
   FRLINE box2%(2, 0), box2%(2, 1)
   FRLINE box2%(3, 0), box2%(3, 1)
   FRLINE box2%(4, 0), box2%(4, 1)
   FRLINE box2%(5, 0), box2%(5, 1)
   FRLINE box2%(0, 0), box2%(0, 1)
   FLINE box2%(6, 0), box2%(6, 1), box2%(7, 0), box2%(7, 1)
   FRLINE box2%(8, 0), box2%(8, 1)
   FRLINE box2%(9, 0), box2%(9, 1)
   FRLINE box2%(10, 0), box2%(10, 1)
   FRLINE box2%(11, 0), box2%(11, 1)
   FRLINE box2%(6, 0), box2%(6, 1)
   FLINE box2%(12, 0), box2%(12, 1), box2%(13, 0), box2%(13, 1)
   FRLINE box2%(14, 0), box2%(14, 1)
   FRLINE box2%(15, 0), box2%(15, 1)
   FRLINE box2%(16, 0), box2%(16, 1)
   FRLINE box2%(13, 0), box2%(13, 1)
   FLINE box2%(15, 0), box2%(15, 1), box2%(17, 0), box2%(17, 1)
   FLINE box2%(14, 0), box2%(14, 1), box2%(18, 0), box2%(18, 1)
   FRLINE box2%(16, 0), box2%(16, 1)
   FLINE box2%(14, 0), box2%(14, 1), box2%(19, 0), box2%(19, 1)
   FRLINE box2%(16, 0), box2%(16, 1)
   FLINE box2%(13, 0), box2%(13, 1), box2%(18, 0), box2%(18, 1)
   FRLINE box2%(15, 0), box2%(15, 1)
   FRLINE box2%(19, 0), box2%(19, 1)
   FRLINE box2%(13, 0), box2%(13, 1)
   FLINE box2%(1, 0), box2%(1, 1), box2%(4, 0), box2%(4, 1)
   FLINE box2%(2, 0), box2%(2, 1), box2%(5, 0), box2%(5, 1)
   FLINE box2%(7, 0), box2%(7, 1), box2%(10, 0), box2%(10, 1)
   FLINE box2%(8, 0), box2%(8, 1), box2%(11, 0), box2%(11, 1)

END SUB

SUB FLINE (p1%, p2%, p3%, p4%)

   DEF SEG = fseg%
   vseg% = buf%
   CALL absolute(vseg%, p1%, p2%, p3%, p4%, flin%)
   xl% = p3%: yl% = p4%
   DEF SEG

END SUB

SUB FRLINE (p1%, p2%)

   DEF SEG = fseg%
   vseg% = buf%
   CALL absolute(vseg%, xl%, yl%, p1%, p2%, flin%)
   DEF SEG
   xl% = p1%: yl% = p2%

END SUB

