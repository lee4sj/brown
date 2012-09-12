/*     */ package kj.dsp;
/*     */ 
/*     */ public class KJFFT
/*     */ {
/*     */   private float[] xre;
/*     */   private float[] xim;
/*     */   private float[] mag;
/*     */   private float[] fftSin;
/*     */   private float[] fftCos;
/*     */   private int[] fftBr;
/*     */   private int ss;
/*     */   private int ss2;
/*     */   private int nu;
/*     */   private int nu1;
/*     */ 
/*     */   public KJFFT(int pSampleSize)
/*     */   {
/*  27 */     this.ss = pSampleSize;
/*  28 */     this.ss2 = (this.ss >> 1);
/*     */ 
/*  30 */     this.xre = new float[this.ss];
/*  31 */     this.xim = new float[this.ss];
/*  32 */     this.mag = new float[this.ss2];
/*     */ 
/*  34 */     this.nu = (int)(Math.log(this.ss) / Math.log(2.0D));
/*  35 */     this.nu1 = (this.nu - 1);
/*     */ 
/*  37 */     prepareFFTTables();
/*     */   }
/*     */ 
/*     */   private int bitrev(int j, int nu)
/*     */   {
/*  43 */     int j1 = j;
/*     */ 
/*  45 */     int k = 0;
/*     */ 
/*  47 */     for (int i = 1; i <= nu; i++) {
/*  48 */       int j2 = j1 >> 1;
/*  49 */       k = (k << 1) + j1 - (j2 << 1);
/*  50 */       j1 = j2;
/*     */     }
/*     */ 
/*  53 */     return k;
/*     */   }
/*     */ 
/*     */   public float[] calculate(float[] pSample)
/*     */   {
/*  64 */     int n2 = this.ss2;
/*  65 */     int nu1 = this.nu - 1;
/*     */ 
/*  67 */     int wAps = pSample.length / this.ss;
/*     */ 
/*  73 */     int a = 0; for (int b = 0; a < pSample.length; b++) {
/*  74 */       this.xre[b] = pSample[a];
/*  75 */       this.xim[b] = 0.0F;
/*     */ 
/*  73 */       a += wAps;
/*     */     }
/*     */ 
/*  79 */     int x = 0;
/*     */ 
/*  81 */     for (int l = 1; l <= this.nu; l++)
/*     */     {
/*  83 */       int k = 0;
/*     */ 
/*  85 */       while (k < this.ss)
/*     */       {
/*  87 */         for (int i = 1; i <= n2; i++)
/*     */         {
/*  90 */           float c = this.fftCos[x];
/*  91 */           float s = this.fftSin[x];
/*     */ 
/*  93 */           int kn2 = k + n2;
/*     */ 
/*  95 */           float tr = this.xre[kn2] * c + this.xim[kn2] * s;
/*  96 */           float ti = this.xim[kn2] * c - this.xre[kn2] * s;
/*     */ 
/*  98 */           this.xre[k] -= tr;
/*  99 */           this.xim[k] -= ti;
/* 100 */           this.xre[k] += tr;
/* 101 */           this.xim[k] += ti;
/*     */ 
/* 103 */           k++; x++;
/*     */         }
/*     */ 
/* 107 */         k += n2;
/*     */       }
/*     */ 
/* 111 */       nu1--;
/* 112 */       n2 >>= 1;
/*     */     }
/*     */ 
/* 119 */     for (int k = 0; k < this.ss; k++)
/*     */     {
/* 122 */       int r = this.fftBr[k];
/*     */ 
/* 124 */       if (r <= k)
/*     */         continue;
/* 126 */       float tr = this.xre[k];
/* 127 */       float ti = this.xim[k];
/*     */ 
/* 129 */       this.xre[k] = this.xre[r];
/* 130 */       this.xim[k] = this.xim[r];
/* 131 */       this.xre[r] = tr;
/* 132 */       this.xim[r] = ti;
/*     */     }
/*     */ 
/* 139 */     this.mag[0] = ((float)Math.sqrt(this.xre[0] * this.xre[0] + this.xim[0] * this.xim[0]) / this.ss);
/*     */ 
/* 141 */     for (int i = 1; i < this.ss2; i++) {
/* 142 */       this.mag[i] = (2.0F * (float)Math.sqrt(this.xre[i] * this.xre[i] + this.xim[i] * this.xim[i]) / this.ss);
/*     */     }
/*     */ 
/* 145 */     return this.mag;
/*     */   }
/*     */ 
/*     */   private void prepareFFTTables()
/*     */   {
/* 151 */     int n2 = this.ss2;
/* 152 */     int nu1 = this.nu - 1;
/*     */ 
/* 155 */     this.fftSin = new float[this.nu * n2];
/* 156 */     this.fftCos = new float[this.nu * n2];
/*     */ 
/* 159 */     int k = 0; int x = 0;
/*     */ 
/* 162 */     for (int l = 1; l <= this.nu; l++)
/*     */     {
/* 164 */       while (k < this.ss)
/*     */       {
/* 166 */         for (int i = 1; i <= n2; i++)
/*     */         {
/* 168 */           float p = bitrev(k >> nu1, this.nu);
/*     */ 
/* 170 */           float arg = 6.283186F * p / this.ss;
/*     */ 
/* 172 */           this.fftSin[x] = (float)Math.sin(arg);
/* 173 */           this.fftCos[x] = (float)Math.cos(arg);
/*     */ 
/* 175 */           k++;
/* 176 */           x++;
/*     */         }
/*     */ 
/* 180 */         k += n2;
/*     */       }
/*     */ 
/* 184 */       k = 0;
/*     */ 
/* 186 */       nu1--;
/* 187 */       n2 >>= 1;
/*     */     }
/*     */ 
/* 192 */     this.fftBr = new int[this.ss];
/*     */ 
/* 194 */     for (k = 0; k < this.ss; k++)
/* 195 */       this.fftBr[k] = bitrev(k, this.nu);
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.dsp.KJFFT
 * JD-Core Version:    0.6.0
 */