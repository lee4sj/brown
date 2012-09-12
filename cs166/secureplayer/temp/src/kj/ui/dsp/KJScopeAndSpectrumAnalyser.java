/*     */ package kj.ui.dsp;
/*     */ 
/*     */ import java.awt.Color;
/*     */ import java.awt.Component;
/*     */ import java.awt.Cursor;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.Graphics;
/*     */ import java.awt.GraphicsConfiguration;
/*     */ import java.awt.Image;
/*     */ import java.awt.event.MouseAdapter;
/*     */ import java.awt.event.MouseEvent;
/*     */ import kj.dsp.KJDigitalSignalProcessor;
/*     */ import kj.dsp.KJFFT;
/*     */ 
/*     */ public class KJScopeAndSpectrumAnalyser extends Component
/*     */   implements KJDigitalSignalProcessor
/*     */ {
/*     */   public static final int DISPLAY_MODE_SCOPE = 0;
/*     */   public static final int DISPLAY_MODE_SPECTRUM_ANALYSER = 1;
/*     */   public static final int DISPLAY_MODE_VU_METER = 2;
/*     */   public static final int DEFAULT_WIDTH = 256;
/*     */   public static final int DEFAULT_HEIGHT = 128;
/*     */   public static final int DEFAULT_SPECTRUM_ANALYSER_FFT_SAMPLE_SIZE = 512;
/*     */   public static final int DEFAULT_SPECTRUM_ANALYSER_BAND_COUNT = 20;
/*     */   public static final float DEFAULT_SPECTRUM_ANALYSER_DECAY = 0.03F;
/*  41 */   public static final Color DEFAULT_BACKGROUND_COLOR = new Color(0, 0, 128);
/*  42 */   public static final Color DEFAULT_SCOPE_COLOR = new Color(255, 192, 0);
/*     */   public static final float DEFAULT_VU_METER_DECAY = 0.02F;
/*     */   private Image bi;
/*  48 */   private int displayMode = 0;
/*     */ 
/*  50 */   private Color scopeColor = DEFAULT_SCOPE_COLOR;
/*  51 */   private Color[] spectrumAnalyserColors = getDefaultSpectrumAnalyserColors();
/*     */   private int width;
/*     */   private int height;
/*     */   private int height_2;
/*     */   private KJFFT fft;
/*     */   private float[] old_FFT;
/*     */   private int saFFTSampleSize;
/*     */   private int saBands;
/*     */   private float saColorScale;
/*     */   private float saMultiplier;
/*  65 */   private float saDecay = 0.03F;
/*     */   private float sad;
/*     */   private float oldLeft;
/*     */   private float oldRight;
/*  74 */   private float vuDecay = 0.02F;
/*     */   private float vuColorScale;
/*  78 */   private long lfu = 0L;
/*  79 */   private int fc = 0;
/*     */   private int fps;
/*  81 */   private boolean showFPS = false;
/*     */ 
/*     */   public KJScopeAndSpectrumAnalyser()
/*     */   {
/*  86 */     initialize();
/*     */   }
/*     */ 
/*     */   private void computeColorScale() {
/*  90 */     this.saColorScale = (this.spectrumAnalyserColors.length / this.height * 2.0F);
/*  91 */     this.vuColorScale = (this.spectrumAnalyserColors.length / (this.width - 32) * 2.0F);
/*     */   }
/*     */ 
/*     */   private void computeSAMultiplier() {
/*  95 */     this.saMultiplier = (this.saFFTSampleSize / 2 / this.saBands);
/*     */   }
/*     */ 
/*     */   private void drawScope(Graphics pGrp, float[] pSample)
/*     */   {
/* 100 */     pGrp.setColor(this.scopeColor);
/*     */ 
/* 102 */     int wLas = (int)(pSample[0] * this.height_2) + this.height_2;
/* 103 */     int wSt = 2;
/*     */ 
/* 105 */     int a = wSt; for (int c = 0; c < this.width; c++)
/*     */     {
/* 107 */       int wAs = (int)(pSample[a] * this.height_2) + this.height_2;
/*     */ 
/* 109 */       pGrp.drawLine(
/* 110 */         c, 
/* 111 */         wLas, 
/* 112 */         c + 1, 
/* 113 */         wAs);
/*     */ 
/* 115 */       wLas = wAs;
/*     */ 
/* 105 */       a += wSt;
/*     */     }
/*     */   }
/*     */ 
/*     */   private void drawSpectrumAnalyser(Graphics pGrp, float[] pSample, float pFrrh)
/*     */   {
/* 123 */     float c = 0.0F;
/* 124 */     float[] wFFT = this.fft.calculate(pSample);
/*     */ 
/* 126 */     float wSadfrr = this.saDecay * pFrrh;
/* 127 */     float wBw = this.width / this.saBands;
/*     */ 
/* 129 */     int a = 0; for (int bd = 0; bd < this.saBands; bd++)
/*     */     {
/* 131 */       float wFs = 0.0F;
/*     */ 
/* 134 */       for (int b = 0; b < this.saMultiplier; b++) {
/* 135 */         wFs += wFFT[(a + b)];
/*     */       }
/*     */ 
/* 139 */       wFs *= (float)Math.log(bd + 2);
/*     */ 
/* 141 */       if (wFs > 1.0F) {
/* 142 */         wFs = 1.0F;
/*     */       }
/*     */ 
/* 146 */       if (wFs >= this.old_FFT[a] - wSadfrr)
/*     */       {
/* 148 */         this.old_FFT[a] = wFs;
/*     */       }
/*     */       else
/*     */       {
/* 152 */         this.old_FFT[a] -= wSadfrr;
/*     */ 
/* 154 */         if (this.old_FFT[a] < 0.0F) {
/* 155 */           this.old_FFT[a] = 0.0F;
/*     */         }
/*     */ 
/* 158 */         wFs = this.old_FFT[a];
/*     */       }
/*     */ 
/* 162 */       drawSpectrumAnalyserBar(
/* 163 */         pGrp, 
/* 164 */         (int)c, 
/* 165 */         this.height, 
/* 166 */         (int)wBw - 1, 
/* 167 */         (int)(wFs * this.height));
/*     */ 
/* 169 */       c += wBw;
/*     */ 
/* 129 */       a = (int)(a + this.saMultiplier);
/*     */     }
/*     */   }
/*     */ 
/*     */   private void drawVUMeter(Graphics pGrp, float[] pLeft, float[] pRight, float pFrrh)
/*     */   {
/* 177 */     float wLeft = 0.0F;
/* 178 */     float wRight = 0.0F;
/*     */ 
/* 180 */     float wSadfrr = this.vuDecay * pFrrh;
/*     */ 
/* 182 */     for (int a = 0; a < pLeft.length; a++)
/*     */     {
/* 184 */       wLeft += Math.abs(pLeft[a]);
/* 185 */       wRight += Math.abs(pRight[a]);
/*     */     }
/*     */ 
/* 189 */     wLeft = wLeft * 2.0F / pLeft.length;
/* 190 */     wRight = wRight * 2.0F / pRight.length;
/*     */ 
/* 192 */     if (wLeft > 1.0F) {
/* 193 */       wLeft = 1.0F;
/*     */     }
/*     */ 
/* 196 */     if (wRight > 1.0F) {
/* 197 */       wRight = 1.0F;
/*     */     }
/*     */ 
/* 208 */     if (wLeft >= this.oldLeft - wSadfrr) {
/* 209 */       this.oldLeft = wLeft;
/*     */     }
/*     */     else {
/* 212 */       this.oldLeft -= wSadfrr;
/*     */ 
/* 214 */       if (this.oldLeft < 0.0F) {
/* 215 */         this.oldLeft = 0.0F;
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 220 */     if (wRight >= this.oldRight - wSadfrr) {
/* 221 */       this.oldRight = wRight;
/*     */     }
/*     */     else {
/* 224 */       this.oldRight -= wSadfrr;
/*     */ 
/* 226 */       if (this.oldRight < 0.0F) {
/* 227 */         this.oldRight = 0.0F;
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 232 */     int wHeight = (this.height >> 1) - 24;
/*     */ 
/* 234 */     drawVolumeMeterBar(pGrp, 16, 16, (int)(this.oldLeft * (this.width - 32)), wHeight);
/*     */ 
/* 236 */     drawVolumeMeterBar(pGrp, 16, wHeight + 32, (int)(this.oldRight * (this.width - 32)), wHeight);
/*     */   }
/*     */ 
/*     */   private void drawSpectrumAnalyserBar(Graphics pGraphics, int pX, int pY, int pWidth, int pHeight)
/*     */   {
/* 247 */     float c = 0.0F;
/*     */ 
/* 249 */     for (int a = pY; a >= pY - pHeight; a -= 2)
/*     */     {
/* 251 */       c += this.saColorScale;
/*     */ 
/* 253 */       if (c < 256.0F) {
/* 254 */         pGraphics.setColor(this.spectrumAnalyserColors[(int)c]);
/*     */       }
/*     */ 
/* 257 */       pGraphics.fillRect(
/* 258 */         pX, 
/* 259 */         a, 
/* 260 */         pWidth, 
/* 261 */         1);
/*     */     }
/*     */   }
/*     */ 
/*     */   private void drawVolumeMeterBar(Graphics pGraphics, int pX, int pY, int pWidth, int pHeight)
/*     */   {
/* 269 */     float c = 0.0F;
/*     */ 
/* 271 */     for (int a = pX; a <= pX + pWidth; a += 2)
/*     */     {
/* 273 */       c += this.vuColorScale;
/*     */ 
/* 275 */       if (c < 256.0F) {
/* 276 */         pGraphics.setColor(this.spectrumAnalyserColors[(int)c]);
/*     */       }
/*     */ 
/* 279 */       pGraphics.fillRect(
/* 280 */         a, 
/* 281 */         pY, 
/* 282 */         1, 
/* 283 */         pHeight);
/*     */     }
/*     */   }
/*     */ 
/*     */   private synchronized Image getDoubleBuffer()
/*     */   {
/* 291 */     if ((this.bi == null) || (this.bi.getWidth(null) != getSize().width) || (this.bi.getHeight(null) != getSize().height))
/*     */     {
/* 293 */       this.width = getSize().width;
/* 294 */       this.height = getSize().height;
/* 295 */       this.height_2 = (this.height >> 1);
/*     */ 
/* 297 */       computeColorScale();
/*     */ 
/* 299 */       this.bi = getGraphicsConfiguration()
/* 301 */         .createCompatibleVolatileImage(this.width, 
/* 301 */         this.height);
/*     */     }
/*     */ 
/* 305 */     return this.bi;
/*     */   }
/*     */ 
/*     */   public static Color[] getDefaultSpectrumAnalyserColors()
/*     */   {
/* 311 */     Color[] wColors = new Color[256];
/*     */ 
/* 313 */     for (int a = 0; a < 128; a++) {
/* 314 */       wColors[a] = new Color(0, (a >> 1) + 192, 0);
/*     */     }
/*     */ 
/* 317 */     for (int a = 0; a < 64; a++) {
/* 318 */       wColors[(a + 128)] = new Color(a << 2, 255, 0);
/*     */     }
/*     */ 
/* 321 */     for (int a = 0; a < 64; a++) {
/* 322 */       wColors[(a + 192)] = new Color(255, 255 - (a << 2), 0);
/*     */     }
/*     */ 
/* 325 */     return wColors;
/*     */   }
/*     */ 
/*     */   public int getDisplayMode()
/*     */   {
/* 333 */     return this.displayMode;
/*     */   }
/*     */ 
/*     */   public int getSpectrumAnalyserBandCount()
/*     */   {
/* 340 */     return this.saBands;
/*     */   }
/*     */ 
/*     */   public float getSpectrumAnalyserDecay()
/*     */   {
/* 347 */     return this.saDecay;
/*     */   }
/*     */ 
/*     */   public Color getScopeColor()
/*     */   {
/* 355 */     return this.scopeColor;
/*     */   }
/*     */ 
/*     */   public Color[] getSpectrumAnalyserColors()
/*     */   {
/* 362 */     return this.spectrumAnalyserColors;
/*     */   }
/*     */ 
/*     */   private void initialize()
/*     */   {
/* 367 */     setSize(256, 128);
/*     */ 
/* 369 */     setBackground(DEFAULT_BACKGROUND_COLOR);
/*     */ 
/* 371 */     prepareDisplayToggleListener();
/*     */ 
/* 373 */     setSpectrumAnalyserBandCount(20);
/* 374 */     setSpectrumAnalyserFFTSampleSize(512);
/*     */   }
/*     */ 
/*     */   public boolean isShowingFPS()
/*     */   {
/* 382 */     return this.showFPS;
/*     */   }
/*     */ 
/*     */   public void paint(Graphics pGraphics) {
/* 386 */     pGraphics.drawImage(getDoubleBuffer(), 0, 0, null);
/*     */   }
/*     */ 
/*     */   private void prepareDisplayToggleListener()
/*     */   {
/* 391 */     setCursor(Cursor.getPredefinedCursor(12));
/*     */ 
/* 393 */     addMouseListener(
/* 394 */       new MouseAdapter()
/*     */     {
/*     */       public void mouseClicked(MouseEvent pEvent) {
/* 397 */         if (pEvent.getButton() == 1)
/*     */         {
/* 399 */           if (KJScopeAndSpectrumAnalyser.this.displayMode + 1 > 2)
/* 400 */             KJScopeAndSpectrumAnalyser.this.displayMode = 0;
/*     */           else
/* 402 */             KJScopeAndSpectrumAnalyser.this.displayMode += 1;
/*     */         }
/*     */       }
/*     */     });
/*     */   }
/*     */ 
/*     */   public synchronized void process(float[] pLeft, float[] pRight, float pFrameRateRatioHint)
/*     */   {
/* 418 */     Graphics wGrp = getDoubleBuffer().getGraphics();
/*     */ 
/* 420 */     wGrp.setColor(getBackground());
/* 421 */     wGrp.fillRect(0, 0, getSize().width, getSize().height);
/*     */ 
/* 423 */     switch (this.displayMode)
/*     */     {
/*     */     case 0:
/* 426 */       drawScope(wGrp, stereoMerge(pLeft, pRight));
/* 427 */       break;
/*     */     case 1:
/* 430 */       drawSpectrumAnalyser(wGrp, stereoMerge(pLeft, pRight), pFrameRateRatioHint);
/* 431 */       break;
/*     */     case 2:
/* 434 */       drawVUMeter(wGrp, pLeft, pRight, pFrameRateRatioHint);
/*     */     }
/*     */ 
/* 440 */     if (this.showFPS)
/*     */     {
/* 443 */       if (System.currentTimeMillis() >= this.lfu + 1000L)
/*     */       {
/* 445 */         this.lfu = System.currentTimeMillis();
/* 446 */         this.fps = this.fc;
/* 447 */         this.fc = 0;
/*     */       }
/*     */ 
/* 451 */       this.fc += 1;
/*     */ 
/* 453 */       wGrp.setColor(Color.yellow);
/* 454 */       wGrp.drawString("FPS: " + this.fps + " (FRRH: " + pFrameRateRatioHint + ")", 0, this.height - 1);
/*     */     }
/*     */ 
/* 458 */     getGraphics().drawImage(getDoubleBuffer(), 0, 0, null);
/*     */   }
/*     */ 
/*     */   public synchronized void setDisplayMode(int pMode)
/*     */   {
/* 477 */     this.displayMode = pMode;
/*     */   }
/*     */ 
/*     */   public synchronized void setScopeColor(Color pColor)
/*     */   {
/* 486 */     this.scopeColor = pColor;
/*     */   }
/*     */ 
/*     */   public synchronized void setShowFPS(boolean pState)
/*     */   {
/* 496 */     this.showFPS = pState;
/*     */   }
/*     */ 
/*     */   public synchronized void setSpectrumAnalyserBandCount(int pCount)
/*     */   {
/* 505 */     this.saBands = pCount;
/* 506 */     computeSAMultiplier();
/*     */   }
/*     */ 
/*     */   public synchronized void setSpectrumAnalyserDecay(float pDecay)
/*     */   {
/* 515 */     this.saDecay = pDecay;
/*     */   }
/*     */ 
/*     */   public synchronized void setSpectrumAnalyserColors(Color[] pColors)
/*     */   {
/* 524 */     this.spectrumAnalyserColors = pColors;
/* 525 */     computeColorScale();
/*     */   }
/*     */ 
/*     */   public synchronized void setSpectrumAnalyserFFTSampleSize(int pSize)
/*     */   {
/* 536 */     this.saFFTSampleSize = pSize;
/*     */ 
/* 538 */     this.fft = new KJFFT(this.saFFTSampleSize);
/* 539 */     this.old_FFT = new float[this.saFFTSampleSize];
/*     */ 
/* 541 */     computeSAMultiplier();
/*     */   }
/*     */ 
/*     */   private float[] stereoMerge(float[] pLeft, float[] pRight)
/*     */   {
/* 547 */     for (int a = 0; a < pLeft.length; a++) {
/* 548 */       pLeft[a] = ((pLeft[a] + pRight[a]) / 2.0F);
/*     */     }
/*     */ 
/* 551 */     return pLeft;
/*     */   }
/*     */ 
/*     */   public void update(Graphics pGraphics)
/*     */   {
/* 557 */     paint(pGraphics);
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.dsp.KJScopeAndSpectrumAnalyser
 * JD-Core Version:    0.6.0
 */