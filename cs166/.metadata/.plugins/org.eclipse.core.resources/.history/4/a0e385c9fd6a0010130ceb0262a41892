/*     */ package kj.dsp;
/*     */ 
/*     */ import java.io.PrintStream;
/*     */ import java.util.ArrayList;
/*     */ import javax.sound.sampled.AudioFormat;
/*     */ import javax.sound.sampled.SourceDataLine;
/*     */ import kj.audio.KJAudioDataConsumer;
/*     */ import kj.ui.KJDSPDialog;
/*     */ import kj.ui.KJJukeBox;
/*     */ 
/*     */ public class KJDigitalSignalProcessingAudioDataConsumer
/*     */   implements KJAudioDataConsumer
/*     */ {
/*     */   private static final int DEFAULT_SAMPLE_SIZE = 2048;
/*     */   private static final int DEFAULT_FPS = 70;
/*  27 */   private Object readWriteLock = new Object();
/*     */   private SourceDataLine sourceDataLine;
/*     */   private int sampleSize;
/*     */   private long fpsAsNS;
/*     */   private long desiredFpsAsNS;
/*     */   private byte[] audioDataBuffer;
/*     */   private float[] left;
/*     */   private float[] right;
/*     */   private int position;
/*     */   private long offset;
/*     */   private SignalProcessor signalProcessor;
/*  46 */   private ArrayList dsps = new ArrayList();
/*     */ 
/*     */   public KJDigitalSignalProcessingAudioDataConsumer()
/*     */   {
/*  53 */     this(2048, 70);
/*     */   }
/*     */ 
/*     */   public KJDigitalSignalProcessingAudioDataConsumer(int pSampleSize, int pFramesPerSecond)
/*     */   {
/*  62 */     this.sampleSize = pSampleSize;
/*  63 */     this.desiredFpsAsNS = (1000000000L / pFramesPerSecond);
/*  64 */     this.fpsAsNS = this.desiredFpsAsNS;
/*     */   }
/*     */ 
/*     */   public void add(KJDigitalSignalProcessor pSignalProcessor)
/*     */   {
/*  75 */     this.dsps.add(pSignalProcessor);
/*     */   }
/*     */ 
/*     */   public void remove(KJDigitalSignalProcessor pSignalProcessor)
/*     */   {
/*  84 */     this.dsps.remove(pSignalProcessor);
/*     */   }
/*     */ 
/*     */   public synchronized void start(SourceDataLine pSdl)
/*     */   {
/*  95 */     if (this.signalProcessor != null) {
/*  96 */       stop();
/*     */     }
/*     */ 
/*  99 */     if (this.signalProcessor == null)
/*     */     {
/* 103 */       this.sourceDataLine = pSdl;
/*     */ 
/* 107 */       this.audioDataBuffer = new byte[pSdl.getBufferSize() << 1];
/*     */ 
/* 109 */       this.left = new float[this.sampleSize];
/* 110 */       this.right = new float[this.sampleSize];
/*     */ 
/* 112 */       this.position = 0;
/* 113 */       this.offset = 0L;
/*     */ 
/* 115 */       this.signalProcessor = new SignalProcessor();
/*     */ 
/* 117 */       new Thread(this.signalProcessor).start();
/*     */     }
/*     */   }
/*     */ 
/*     */   public synchronized void stop()
/*     */   {
/* 128 */     if (this.signalProcessor != null)
/*     */     {
/* 130 */       this.signalProcessor.stop();
/* 131 */       this.signalProcessor = null;
/*     */ 
/* 133 */       this.audioDataBuffer = null;
/* 134 */       this.sourceDataLine = null;
/*     */     }
/*     */   }
/*     */ 
/*     */   private void storeAudioData(byte[] pAudioData, int pOffset, int pLength)
/*     */   {
/* 142 */     synchronized (this.readWriteLock)
/*     */     {
/* 144 */       if (this.audioDataBuffer == null) {
/* 145 */         return;
/*     */       }
/*     */ 
/* 148 */       int wOverrun = 0;
/*     */ 
/* 150 */       if (this.position + pLength > this.audioDataBuffer.length - 1)
/*     */       {
/* 152 */         wOverrun = this.position + pLength - this.audioDataBuffer.length;
/* 153 */         pLength = this.audioDataBuffer.length - this.position;
/*     */       }
/*     */ 
/* 159 */       if (wOverrun > 0)
/*     */       {
/* 162 */         this.position = wOverrun;
/*     */       }
/*     */       else {
/* 165 */         this.position += pLength;
/*     */       }
/*     */ 
/* 168 */       KJJukeBox.getDSPDialog().setDSPBufferInfo(
/* 169 */         this.position, 
/* 170 */         pOffset, 
/* 171 */         pLength, 
/* 172 */         this.audioDataBuffer.length);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void writeAudioData(byte[] pAudioData, boolean encrypted)
/*     */   {
/* 182 */     storeAudioData(pAudioData, 0, pAudioData.length);
/*     */   }
/*     */ 
/*     */   public void writeAudioData(byte[] pAudioData, int pOffset, int pLength, boolean encrypted)
/*     */   {
/* 189 */     storeAudioData(pAudioData, pOffset, pLength);
/*     */   }
/*     */ 
/*     */   private class SignalProcessor implements Runnable
/*     */   {
/* 194 */     boolean process = true;
/*     */ 
/* 196 */     long lfp = 0L;
/*     */ 
/* 201 */     int frameSize = KJDigitalSignalProcessingAudioDataConsumer.this.sourceDataLine.getFormat().getFrameSize();
/*     */ 
/*     */     public SignalProcessor() {
/*     */     }
/*     */     private int calculateSamplePosition() {
/* 206 */       synchronized (KJDigitalSignalProcessingAudioDataConsumer.this.readWriteLock)
/*     */       {
/* 208 */         long wFp = KJDigitalSignalProcessingAudioDataConsumer.this.sourceDataLine.getLongFramePosition();
/* 209 */         long wNfp = this.lfp;
/*     */ 
/* 211 */         this.lfp = wFp;
/*     */ 
/* 213 */         int wSdp = (int)(wNfp * this.frameSize - KJDigitalSignalProcessingAudioDataConsumer.this.audioDataBuffer.length * KJDigitalSignalProcessingAudioDataConsumer.this.offset);
/*     */ 
/* 215 */         KJJukeBox.getDSPDialog().setOutputPositionInfo(
/* 216 */           wFp, 
/* 217 */           wFp - wNfp, 
/* 218 */           wSdp);
/*     */ 
/* 220 */         return wSdp;
/*     */       }
/*     */     }
/*     */ 
/*     */     public void run()
/*     */     {
/* 228 */       while (this.process)
/*     */       {
/*     */         try
/*     */         {
/* 232 */           long wStn = System.nanoTime();
/*     */ 
/* 234 */           int wSdp = calculateSamplePosition();
/*     */ 
/* 236 */           if (wSdp > 0)
/*     */           {
/* 238 */             int c = wSdp;
/*     */ 
/* 240 */             for (int a = 0; a < KJDigitalSignalProcessingAudioDataConsumer.this.sampleSize; c += 4)
/*     */             {
/* 242 */               if (c + 3 >= KJDigitalSignalProcessingAudioDataConsumer.this.audioDataBuffer.length) {
/* 243 */                 KJDigitalSignalProcessingAudioDataConsumer.this.offset += 1L;
/* 244 */                 c = (c + 3) % KJDigitalSignalProcessingAudioDataConsumer.this.audioDataBuffer.length;
/*     */               }
/*     */ 
/* 252 */               KJDigitalSignalProcessingAudioDataConsumer.this.left[a] = (((KJDigitalSignalProcessingAudioDataConsumer.access$2(KJDigitalSignalProcessingAudioDataConsumer.this)[(c + 1)] << 8) + KJDigitalSignalProcessingAudioDataConsumer.access$2(KJDigitalSignalProcessingAudioDataConsumer.this)[c]) / 32767.0F);
/* 253 */               KJDigitalSignalProcessingAudioDataConsumer.this.right[a] = (((KJDigitalSignalProcessingAudioDataConsumer.access$2(KJDigitalSignalProcessingAudioDataConsumer.this)[(c + 3)] << 8) + KJDigitalSignalProcessingAudioDataConsumer.access$2(KJDigitalSignalProcessingAudioDataConsumer.this)[(c + 2)]) / 32767.0F);
/*     */ 
/* 240 */               a++;
/*     */             }
/*     */ 
/*     */           }
/*     */ 
/* 260 */           for (int a = 0; a < KJDigitalSignalProcessingAudioDataConsumer.this.dsps.size(); a++)
/*     */           {
/* 265 */             float wFrr = (float)KJDigitalSignalProcessingAudioDataConsumer.this.fpsAsNS / (float)KJDigitalSignalProcessingAudioDataConsumer.this.desiredFpsAsNS;
/*     */             try
/*     */             {
/* 268 */               ((KJDigitalSignalProcessor)KJDigitalSignalProcessingAudioDataConsumer.this.dsps.get(a)).process(KJDigitalSignalProcessingAudioDataConsumer.this.left, KJDigitalSignalProcessingAudioDataConsumer.this.right, wFrr);
/*     */             } catch (Exception pEx) {
/* 270 */               System.err.println("-- DSP Exception: ");
/* 271 */               pEx.printStackTrace();
/*     */             }
/*     */ 
/*     */           }
/*     */ 
/* 280 */           long wDelay = KJDigitalSignalProcessingAudioDataConsumer.this.fpsAsNS - (System.nanoTime() - wStn);
/*     */ 
/* 283 */           if (KJDigitalSignalProcessingAudioDataConsumer.this.dsps.isEmpty()) {
/* 284 */             wDelay = 1000000000L;
/*     */           }
/*     */ 
/* 287 */           if (wDelay > 0L)
/*     */           {
/*     */             try {
/* 290 */               Thread.sleep(wDelay / 1000000L, (int)wDelay % 1000000);
/*     */             }
/*     */             catch (Exception localException1)
/*     */             {
/*     */             }
/*     */ 
/* 296 */             if (KJDigitalSignalProcessingAudioDataConsumer.this.fpsAsNS > KJDigitalSignalProcessingAudioDataConsumer.this.desiredFpsAsNS)
/* 297 */               KJDigitalSignalProcessingAudioDataConsumer.this.fpsAsNS -= wDelay;
/*     */             else {
/* 299 */               KJDigitalSignalProcessingAudioDataConsumer.this.fpsAsNS = KJDigitalSignalProcessingAudioDataConsumer.this.desiredFpsAsNS;
/*     */             }
/*     */ 
/*     */           }
/*     */           else
/*     */           {
/* 305 */             KJDigitalSignalProcessingAudioDataConsumer.this.fpsAsNS += -wDelay;
/*     */             try
/*     */             {
/* 309 */               Thread.sleep(10L);
/*     */             }
/*     */             catch (InterruptedException localInterruptedException)
/*     */             {
/*     */             }
/*     */           }
/*     */         }
/*     */         catch (Exception pEx) {
/* 317 */           System.err.println("- DSP Exception: ");
/* 318 */           pEx.printStackTrace();
/*     */         }
/*     */       }
/*     */     }
/*     */ 
/*     */     public void stop()
/*     */     {
/* 326 */       this.process = false;
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.dsp.KJDigitalSignalProcessingAudioDataConsumer
 * JD-Core Version:    0.6.0
 */