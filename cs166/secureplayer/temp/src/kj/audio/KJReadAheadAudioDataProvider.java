/*     */ package kj.audio;
/*     */ 
/*     */ import java.io.PrintStream;
/*     */ 
/*     */ public class KJReadAheadAudioDataProvider
/*     */   implements KJAudioDataProvider
/*     */ {
/*     */   private static final int DEFAULT_BUFFER_SIZE = 1048576;
/*     */   private KJAudioDataProvider target;
/*     */   private Thread readAheadThread;
/*     */   private byte[] readAheadBuffer;
/*     */   private byte[] frameBuffer;
/*  26 */   private Object readLock = new Object();
/*     */ 
/*  28 */   private int bufferPosition = 0;
/*  29 */   private int readPosition = 0;
/*  30 */   private int framesRead = 0;
/*  31 */   private int waitingDataSize = 0;
/*     */ 
/*  33 */   private boolean disposed = false;
/*     */ 
/*     */   public boolean isEncrypted()
/*     */   {
/*  54 */     return this.target.isEncrypted();
/*     */   }
/*     */ 
/*     */   public int bufferPosition() {
/*  58 */     return this.bufferPosition;
/*     */   }
/*     */ 
/*     */   public int channels() {
/*  62 */     return this.target.channels();
/*     */   }
/*     */ 
/*     */   public int dataBuffered() {
/*  66 */     return this.waitingDataSize;
/*     */   }
/*     */ 
/*     */   public void dispose()
/*     */   {
/*  71 */     this.disposed = true;
/*     */ 
/*  73 */     synchronized (this.readLock) {
/*  74 */       this.readLock.notify();
/*     */     }
/*     */ 
/*  77 */     this.target.dispose();
/*     */   }
/*     */ 
/*     */   public int frameCount()
/*     */   {
/*  82 */     return this.target.frameCount();
/*     */   }
/*     */ 
/*     */   public int frameSize() {
/*  86 */     return this.target.frameSize();
/*     */   }
/*     */ 
/*     */   public int frequency() {
/*  90 */     return this.target.frequency();
/*     */   }
/*     */ 
/*     */   public int sampleSize() {
/*  94 */     return this.target.sampleSize();
/*     */   }
/*     */ 
/*     */   public boolean hasMoreFrames() {
/*  98 */     return this.framesRead < this.target.frameCount();
/*     */   }
/*     */ 
/*     */   public boolean isDisposed() {
/* 102 */     return this.disposed;
/*     */   }
/*     */ 
/*     */   public void writeFrame(KJAudioDataConsumer pConsumer)
/*     */   {
/* 109 */     pConsumer.writeAudioData(this.readAheadBuffer, this.readPosition, this.target.frameSize(), this.target.isEncrypted());
/*     */ 
/* 111 */     this.waitingDataSize -= this.target.frameSize();
/*     */ 
/* 113 */     if (this.waitingDataSize == 0)
/*     */     {
/* 115 */       synchronized (this.readLock) {
/*     */         try {
/* 117 */           this.readLock.wait();
/*     */         }
/*     */         catch (InterruptedException localInterruptedException)
/*     */         {
/*     */         }
/*     */ 
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 129 */     this.readPosition += this.target.frameSize();
/* 130 */     this.framesRead += 1;
/*     */ 
/* 132 */     if (this.readPosition + this.target.frameSize() > this.readAheadBuffer.length)
/* 133 */       this.readPosition = 0;
/*     */   }
/*     */ 
/*     */   public int readPosition()
/*     */   {
/* 139 */     return this.readPosition;
/*     */   }
/*     */ 
/*     */   public void seek(long pPositionInBytes)
/*     */   {
/*     */   }
/*     */ 
/*     */   public long endPosition()
/*     */   {
/* 239 */     return 0L;
/*     */   }
/*     */ 
/*     */   public long startPosition()
/*     */   {
/* 247 */     return 0L;
/*     */   }
/*     */ 
/*     */   public long position()
/*     */   {
/* 255 */     return 0L;
/*     */   }
/*     */ 
/*     */   public long positionInMilliseconds()
/*     */   {
/* 263 */     return 0L;
/*     */   }
/*     */ 
/*     */   private class ReadAheadThread extends Thread
/*     */     implements KJAudioDataConsumer
/*     */   {
/*     */     private byte[] frame;
/*     */     private int offset;
/*     */     private int length;
/*     */ 
/*     */     public ReadAheadThread()
/*     */     {
/* 149 */       setPriority(9);
/*     */     }
/*     */ 
/*     */     public void run()
/*     */     {
/* 155 */       while ((!KJReadAheadAudioDataProvider.this.disposed) && (KJReadAheadAudioDataProvider.this.target.hasMoreFrames()))
/*     */       {
/* 158 */         if (KJReadAheadAudioDataProvider.this.waitingDataSize + KJReadAheadAudioDataProvider.this.target.frameCount() > KJReadAheadAudioDataProvider.this.readAheadBuffer.length)
/*     */         {
/*     */           try {
/* 161 */             sleep(1000L);
/*     */           }
/*     */           catch (InterruptedException localInterruptedException)
/*     */           {
/*     */           }
/*     */ 
/*     */         }
/*     */         else
/*     */         {
/* 183 */           KJReadAheadAudioDataProvider.this.target.writeFrame(this);
/*     */ 
/* 185 */           System.arraycopy(this.frame, this.offset, KJReadAheadAudioDataProvider.this.readAheadBuffer, KJReadAheadAudioDataProvider.this.bufferPosition, this.length);
/*     */ 
/* 187 */           KJReadAheadAudioDataProvider.this.bufferPosition += this.length;
/* 188 */           KJReadAheadAudioDataProvider.this.waitingDataSize += this.length;
/*     */ 
/* 190 */           synchronized (KJReadAheadAudioDataProvider.this.readLock) {
/* 191 */             KJReadAheadAudioDataProvider.this.readLock.notify();
/*     */           }
/*     */ 
/* 194 */           if (KJReadAheadAudioDataProvider.this.bufferPosition + KJReadAheadAudioDataProvider.this.target.frameSize() > KJReadAheadAudioDataProvider.this.readAheadBuffer.length) {
/* 195 */             KJReadAheadAudioDataProvider.this.bufferPosition = 0;
/*     */           }
/*     */         }
/*     */       }
/*     */ 
/* 200 */       System.err.println("- ReadAhead thread disposed!!!");
/*     */     }
/*     */ 
/*     */     public void writeAudioData(byte[] pAudioData, boolean encrypted)
/*     */     {
/* 205 */       if (encrypted) {
/* 206 */         throw new RuntimeException("Tried to read ahead encrypted buffer!");
/*     */       }
/*     */ 
/* 209 */       this.frame = pAudioData;
/* 210 */       this.offset = 0;
/* 211 */       this.length = pAudioData.length;
/*     */     }
/*     */ 
/*     */     public void writeAudioData(byte[] pAudioData, int pOffset, int pLength, boolean encrypted) {
/* 215 */       if (encrypted) {
/* 216 */         throw new RuntimeException("Tried to read ahead encrypted buffer!");
/*     */       }
/*     */ 
/* 219 */       this.frame = pAudioData;
/* 220 */       this.offset = pOffset;
/* 221 */       this.length = pLength;
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.audio.KJReadAheadAudioDataProvider
 * JD-Core Version:    0.6.0
 */