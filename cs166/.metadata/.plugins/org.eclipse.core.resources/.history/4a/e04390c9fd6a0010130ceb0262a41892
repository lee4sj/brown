/*     */ package kj.track;
/*     */ 
/*     */ import java.io.ByteArrayInputStream;
/*     */ import java.io.ByteArrayOutputStream;
/*     */ import java.io.File;
/*     */ import java.io.FileInputStream;
/*     */ import java.io.IOException;
/*     */ import java.io.PrintStream;
/*     */ import java.io.RandomAccessFile;
/*     */ import java.io.Serializable;
/*     */ import javax.crypto.SecretKey;
/*     */ import javax.crypto.SecretKeyFactory;
/*     */ import javax.crypto.spec.DESKeySpec;
/*     */ import javazoom.jl.decoder.Bitstream;
/*     */ import javazoom.jl.decoder.Decoder;
/*     */ import javazoom.jl.decoder.Header;
/*     */ import kj.audio.KJAudioDataConsumer;
/*     */ import kj.audio.KJAudioDataProvider;
/*     */ import kj.audio.KJAudioDataProviderException;
/*     */ import kj.audio.KJSampleBuffer;
/*     */ import kj.ui.KJJukeBox;
/*     */ 
/*     */ public class KJMP3Track extends KJTrack
/*     */ {
/*     */   private static final int BUFFER_SIZE = 4608;
/*  40 */   private static short[] EMPTY_FRAME = new short[2304];
/*     */ 
/*  42 */   private boolean encrypted = false;
/*     */   private KJJukeBox juke_box;
/*     */ 
/*     */   public KJMP3Track()
/*     */   {
/*     */   }
/*     */ 
/*     */   public KJMP3Track(KJTrackList pPlayList, File pMP3File, KJJukeBox box)
/*     */   {
/*  50 */     super(pPlayList, pMP3File);
/*     */ 
/*  52 */     this.juke_box = box;
/*  53 */     if (pMP3File.getName().toLowerCase().endsWith(".mps"))
/*  54 */       this.encrypted = true;
/*     */   }
/*     */ 
/*     */   protected KJAudioDataProvider createAudioDataProvider() {
/*  58 */     return new AudioDataProvider(file());
/*     */   }
/*     */ 
/*     */   public long duration() {
/*  62 */     return ((AudioDataProvider)getAudioDataProvider()).duration();
/*     */   }
/*     */ 
/*     */   private class AudioDataProvider
/*     */     implements KJAudioDataProvider, Serializable
/*     */   {
/*     */     private KJSampleBuffer sampleBuffer;
/*     */     private Bitstream bitstream;
/*     */     private Decoder decoder;
/*     */     private Header header;
/*  76 */     private int averageFrameSize = 0;
/*     */ 
/*  78 */     private int framesRead = 0;
/*     */     private float millisecondsPerFrame;
/*  82 */     private boolean disposed = false;
/*     */ 
/*  84 */     private boolean eos = false;
/*     */     private RandomAccessFile randomAccessFile;
/*  88 */     private Object seekLock = new Object();
/*     */ 
/*     */     public AudioDataProvider(File pFile)
/*     */     {
/*     */       try
/*     */       {
/*  94 */         this.randomAccessFile = new RandomAccessFile(pFile, "r");
/*     */ 
/*  96 */         fis = new FileInputStream(
/*  97 */           this.randomAccessFile.getFD());
/*     */ 
/*  99 */         if (isEncrypted()) {
/* 100 */           ByteArrayOutputStream baos = new ByteArrayOutputStream();
/*     */ 
/* 105 */           String passkey = KJMP3Track.this.juke_box.getPassword();
/* 106 */           if (passkey.length() > 8)
/* 107 */             passkey = passkey.substring(0, 8);
/* 108 */           while (passkey.length() < 8) {
/* 109 */             passkey = passkey.concat("0");
/*     */           }
/* 111 */           byte[] bytes = passkey.getBytes();
/*     */ 
/* 113 */           DESKeySpec specs = new DESKeySpec(bytes);
/* 114 */           SecretKey key = SecretKeyFactory.getInstance("DES")
/* 115 */             .generateSecret(specs);
/*     */ 
/* 117 */           KJMP3Track.this.decrypt(fis, baos, key);
/*     */ 
/* 119 */           ByteArrayInputStream bais = new ByteArrayInputStream(
/* 120 */             baos.toByteArray());
/*     */ 
/* 122 */           this.bitstream = new Bitstream(bais);
/*     */         }
/*     */         else {
/* 125 */           this.bitstream = new Bitstream(fis);
/*     */         }
/* 127 */         this.header = this.bitstream.readFrame();
/*     */ 
/* 129 */         this.millisecondsPerFrame = this.header.ms_per_frame();
/*     */ 
/* 131 */         this.decoder = new Decoder();
/*     */ 
/* 133 */         this.decoder.setOutputBuffer(
/* 134 */           new KJSampleBuffer(this.header.frequency(), 
/* 134 */           this.header.mode() == 3 ? 1 : 2));
/*     */ 
/* 136 */         this.sampleBuffer = 
/* 137 */           ((KJSampleBuffer)this.decoder
/* 137 */           .decodeFrame(this.header, 
/* 137 */           this.bitstream));
/*     */ 
/* 139 */         this.averageFrameSize = (this.header.framesize + 4);
/* 140 */         this.framesRead += 1;
/*     */       }
/*     */       catch (Exception pEx)
/*     */       {
/*     */         FileInputStream fis;
/* 143 */         throw new KJAudioDataProviderException(pEx.toString());
/*     */       }
/*     */     }
/*     */ 
/*     */     public int channels()
/*     */     {
/* 149 */       return this.decoder.getOutputChannels();
/*     */     }
/*     */ 
/*     */     public boolean isEncrypted() {
/* 153 */       return KJMP3Track.this.encrypted;
/*     */     }
/*     */ 
/*     */     public void dispose()
/*     */     {
/*     */       try {
/* 159 */         this.bitstream.close();
/*     */       }
/*     */       catch (Exception localException)
/*     */       {
/*     */       }
/*     */       try {
/* 165 */         this.randomAccessFile.close();
/*     */       }
/*     */       catch (Exception localException1)
/*     */       {
/*     */       }
/* 170 */       this.disposed = true;
/*     */ 
/* 172 */       this.bitstream = null;
/* 173 */       this.decoder = null;
/* 174 */       this.header = null;
/*     */     }
/*     */ 
/*     */     public long duration()
/*     */     {
/* 179 */       return ()(getFrameCount() * this.millisecondsPerFrame) - 1000L;
/*     */     }
/*     */ 
/*     */     public long endPosition() {
/* 183 */       return KJMP3Track.this.size() - 128L;
/*     */     }
/*     */ 
/*     */     public int frameCount() {
/* 187 */       return (int)getFrameCount();
/*     */     }
/*     */ 
/*     */     public int frameSize() {
/* 191 */       return this.decoder.getOutputBlockSize();
/*     */     }
/*     */ 
/*     */     public int frequency() {
/* 195 */       return this.decoder.getOutputFrequency();
/*     */     }
/*     */ 
/*     */     protected float getAverageFrameSize() {
/* 199 */       return this.averageFrameSize / this.framesRead;
/*     */     }
/*     */ 
/*     */     protected float getCurrentFrame() {
/* 203 */       return (float)(position() - startPosition()) / 
/* 204 */         getAverageFrameSize();
/*     */     }
/*     */ 
/*     */     protected float getFrameCount() {
/* 208 */       return (float)(KJMP3Track.this.size() - startPosition()) / getAverageFrameSize();
/*     */     }
/*     */ 
/*     */     public boolean hasMoreFrames() {
/* 212 */       return (!this.disposed) && (!this.eos);
/*     */     }
/*     */ 
/*     */     public boolean isDisposed() {
/* 216 */       return this.disposed;
/*     */     }
/*     */ 
/*     */     public long position()
/*     */     {
/*     */       try {
/* 222 */         return this.randomAccessFile.getFilePointer();
/*     */       } catch (IOException pEx) {
/* 224 */         pEx.printStackTrace();
/*     */       }
/*     */ 
/* 227 */       return -1L;
/*     */     }
/*     */ 
/*     */     public long positionInMilliseconds()
/*     */     {
/* 232 */       return ()(getCurrentFrame() * this.millisecondsPerFrame) - 1000L;
/*     */     }
/*     */ 
/*     */     public void seek(long pPosition)
/*     */     {
/* 237 */       synchronized (this.seekLock)
/*     */       {
/*     */         try
/*     */         {
/* 241 */           this.bitstream.closeFrame();
/*     */ 
/* 243 */           this.randomAccessFile.seek(pPosition);
/*     */ 
/* 248 */           long wSsp = 0L;
/*     */           do
/*     */           {
/* 252 */             wSsp = this.randomAccessFile.getFilePointer();
/*     */             try
/*     */             {
/* 256 */               this.bitstream.readFrame();
/* 257 */               this.decoder.decodeFrame(this.header, this.bitstream);
/* 258 */               this.bitstream.closeFrame();
/*     */             }
/*     */             catch (Exception pEx) {
/* 261 */               pEx.printStackTrace();
/*     */             }
/*     */           }
/* 264 */           while (wSsp == pPosition);
/*     */ 
/* 267 */           this.header = this.bitstream.readFrame();
/*     */         }
/*     */         catch (Exception pEx) {
/* 270 */           pEx.printStackTrace();
/*     */         }
/*     */       }
/*     */     }
/*     */ 
/*     */     public long startPosition()
/*     */     {
/* 278 */       return this.bitstream.header_pos();
/*     */     }
/*     */ 
/*     */     public void writeFrame(KJAudioDataConsumer pConsumer)
/*     */     {
/* 283 */       synchronized (this.seekLock)
/*     */       {
/*     */         try
/*     */         {
/* 288 */           pConsumer.writeAudioData(this.sampleBuffer.getBuffer(), 0, 
/* 289 */             this.sampleBuffer.getBufferLength(), KJMP3Track.this.encrypted);
/*     */ 
/* 291 */           this.bitstream.closeFrame();
/*     */ 
/* 294 */           this.header = this.bitstream.readFrame();
/*     */ 
/* 297 */           if (this.header != null)
/*     */           {
/* 299 */             this.averageFrameSize += this.header.framesize + 4;
/* 300 */             this.framesRead += 1;
/*     */           }
/*     */           else
/*     */           {
/* 304 */             this.eos = true;
/* 305 */             return;
/*     */           }
/*     */ 
/* 310 */           this.sampleBuffer = 
/* 311 */             ((KJSampleBuffer)this.decoder
/* 311 */             .decodeFrame(this.header, 
/* 311 */             this.bitstream));
/*     */         }
/*     */         catch (Exception pEx)
/*     */         {
/* 325 */           System.out.println("-- Decoder exception:");
/* 326 */           pEx.printStackTrace();
/*     */         }
/*     */       }
/*     */     }
/*     */ 
/*     */     public int sampleSize()
/*     */     {
/* 337 */       return 16;
/*     */     }
/*     */ 
/*     */     public String toString() {
/* 341 */       return frequency() + " " + channels() + " " + sampleSize();
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.track.KJMP3Track
 * JD-Core Version:    0.6.0
 */