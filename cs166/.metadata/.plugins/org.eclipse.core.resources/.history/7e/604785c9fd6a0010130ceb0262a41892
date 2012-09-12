/*     */ package kj.audio;
/*     */ 
/*     */ import javazoom.jl.decoder.Obuffer;
/*     */ 
/*     */ public class KJSampleBuffer extends Obuffer
/*     */ {
/*     */   private byte[] buffer;
/*     */   private int[] bufferp;
/*     */   private int channels;
/*     */   private int frequency;
/*     */ 
/*     */   public KJSampleBuffer(int pSampleFrequency, int pNumberOfChannels)
/*     */   {
/*  32 */     this.buffer = new byte[2304 * pNumberOfChannels];
/*  33 */     this.bufferp = new int[2];
/*     */ 
/*  38 */     this.channels = pNumberOfChannels;
/*  39 */     this.frequency = pSampleFrequency;
/*     */   }
/*     */ 
/*     */   public int getChannelCount()
/*     */   {
/*  44 */     return this.channels;
/*     */   }
/*     */ 
/*     */   public int getSampleFrequency() {
/*  48 */     return this.frequency;
/*     */   }
/*     */ 
/*     */   public byte[] getBuffer() {
/*  52 */     return this.buffer;
/*     */   }
/*     */ 
/*     */   public int getBufferLength() {
/*  56 */     return this.bufferp[0];
/*     */   }
/*     */ 
/*     */   public void append(int pChannel, short pValue)
/*     */   {
/*  76 */     this.buffer[this.bufferp[pChannel]] = (byte)pValue;
/*  77 */     this.buffer[(this.bufferp[pChannel] + 1)] = (byte)(pValue >>> 8);
/*     */ 
/*  79 */     this.bufferp[pChannel] += (this.channels << 1);
/*     */   }
/*     */ 
/*     */   public void appendSamples(int pChannel, float[] pSamples)
/*     */   {
/*  85 */     int wPosition = this.bufferp[pChannel];
/*     */ 
/*  87 */     for (int i = 0; i < 32; i++)
/*     */     {
/*  89 */       float wFloatSample = pSamples[i];
/*     */ 
/*  97 */       wFloatSample = wFloatSample < -32767.0F ? -32767.0F : wFloatSample > 32767.0F ? 32767.0F : wFloatSample;
/*     */ 
/*  99 */       short wSample = (short)(int)wFloatSample;
/*     */ 
/* 101 */       this.buffer[wPosition] = (byte)wSample;
/* 102 */       this.buffer[(wPosition + 1)] = (byte)(wSample >>> 8);
/*     */ 
/* 104 */       wPosition += (this.channels << 1);
/*     */     }
/*     */ 
/* 108 */     this.bufferp[pChannel] = wPosition;
/*     */   }
/*     */ 
/*     */   public void write_buffer(int val)
/*     */   {
/*     */   }
/*     */ 
/*     */   public void close()
/*     */   {
/*     */   }
/*     */ 
/*     */   public void clear_buffer()
/*     */   {
/* 136 */     for (int i = 0; i < this.channels; i++)
/* 137 */       this.bufferp[i] = (i << 1);
/*     */   }
/*     */ 
/*     */   public void set_stop_flag()
/*     */   {
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.audio.KJSampleBuffer
 * JD-Core Version:    0.6.0
 */