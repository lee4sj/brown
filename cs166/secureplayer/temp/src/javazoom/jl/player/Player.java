/*     */ package javazoom.jl.player;
/*     */ 
/*     */ import java.io.InputStream;
/*     */ import javazoom.jl.decoder.Bitstream;
/*     */ import javazoom.jl.decoder.BitstreamException;
/*     */ import javazoom.jl.decoder.Decoder;
/*     */ import javazoom.jl.decoder.Header;
/*     */ import javazoom.jl.decoder.JavaLayerException;
/*     */ import javazoom.jl.decoder.SampleBuffer;
/*     */ 
/*     */ public class Player
/*     */ {
/*  47 */   private int frame = 0;
/*     */   private Bitstream bitstream;
/*     */   private Decoder decoder;
/*     */   private AudioDevice audio;
/*  68 */   private boolean closed = false;
/*     */ 
/*  73 */   private boolean complete = false;
/*     */ 
/*  75 */   private int lastPosition = 0;
/*     */ 
/*     */   public Player(InputStream stream)
/*     */     throws JavaLayerException
/*     */   {
/*  82 */     this(stream, null);
/*     */   }
/*     */ 
/*     */   public Player(InputStream stream, AudioDevice device) throws JavaLayerException
/*     */   {
/*  87 */     this.bitstream = new Bitstream(stream);
/*  88 */     this.decoder = new Decoder();
/*     */ 
/*  90 */     if (device != null)
/*     */     {
/*  92 */       this.audio = device;
/*     */     }
/*     */     else
/*     */     {
/*  96 */       FactoryRegistry r = FactoryRegistry.systemRegistry();
/*  97 */       this.audio = r.createAudioDevice();
/*     */     }
/*  99 */     this.audio.open(this.decoder);
/*     */   }
/*     */ 
/*     */   public void play() throws JavaLayerException
/*     */   {
/* 104 */     play(2147483647);
/*     */   }
/*     */ 
/*     */   public boolean play(int frames)
/*     */     throws JavaLayerException
/*     */   {
/* 116 */     boolean ret = true;
/*     */ 
/* 118 */     while ((frames-- > 0) && (ret))
/*     */     {
/* 120 */       ret = decodeFrame();
/*     */     }
/*     */ 
/* 123 */     if (!ret)
/*     */     {
/* 126 */       AudioDevice out = this.audio;
/* 127 */       if (out != null)
/*     */       {
/* 129 */         out.flush();
/* 130 */         synchronized (this)
/*     */         {
/* 132 */           this.complete = (!this.closed);
/* 133 */           close();
/*     */         }
/*     */       }
/*     */     }
/* 137 */     return ret;
/*     */   }
/*     */ 
/*     */   public synchronized void close()
/*     */   {
/* 146 */     AudioDevice out = this.audio;
/* 147 */     if (out != null)
/*     */     {
/* 149 */       this.closed = true;
/* 150 */       this.audio = null;
/*     */ 
/* 153 */       out.close();
/* 154 */       this.lastPosition = out.getPosition();
/*     */       try
/*     */       {
/* 157 */         this.bitstream.close();
/*     */       }
/*     */       catch (BitstreamException localBitstreamException)
/*     */       {
/*     */       }
/*     */     }
/*     */   }
/*     */ 
/*     */   public synchronized boolean isComplete()
/*     */   {
/* 173 */     return this.complete;
/*     */   }
/*     */ 
/*     */   public int getPosition()
/*     */   {
/* 184 */     int position = this.lastPosition;
/*     */ 
/* 186 */     AudioDevice out = this.audio;
/* 187 */     if (out != null)
/*     */     {
/* 189 */       position = out.getPosition();
/*     */     }
/* 191 */     return position;
/*     */   }
/*     */ 
/*     */   protected boolean decodeFrame()
/*     */     throws JavaLayerException
/*     */   {
/*     */     try
/*     */     {
/* 203 */       AudioDevice out = this.audio;
/* 204 */       if (out == null) {
/* 205 */         return false;
/*     */       }
/* 207 */       Header h = this.bitstream.readFrame();
/*     */ 
/* 209 */       if (h == null) {
/* 210 */         return false;
/*     */       }
/*     */ 
/* 213 */       SampleBuffer output = (SampleBuffer)this.decoder.decodeFrame(h, this.bitstream);
/*     */ 
/* 215 */       synchronized (this)
/*     */       {
/* 217 */         out = this.audio;
/* 218 */         if (out != null)
/*     */         {
/* 220 */           out.write(output.getBuffer(), 0, output.getBufferLength());
/*     */         }
/*     */       }
/*     */ 
/* 224 */       this.bitstream.closeFrame();
/*     */     }
/*     */     catch (RuntimeException ex)
/*     */     {
/* 228 */       throw new JavaLayerException("Exception decoding audio frame", ex);
/*     */     }
/*     */ 
/* 247 */     return true;
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.player.Player
 * JD-Core Version:    0.6.0
 */