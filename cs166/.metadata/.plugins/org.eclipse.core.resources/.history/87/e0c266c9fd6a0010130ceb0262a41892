/*     */ package javazoom.jl.converter;
/*     */ 
/*     */ import javazoom.jl.decoder.Obuffer;
/*     */ 
/*     */ public class WaveFileObuffer extends Obuffer
/*     */ {
/*     */   private short[] buffer;
/*     */   private short[] bufferp;
/*     */   private int channels;
/*     */   private WaveFile outWave;
/*  85 */   short[] myBuffer = new short[2];
/*     */ 
/*     */   public WaveFileObuffer(int number_of_channels, int freq, String FileName)
/*     */   {
/*  58 */     if (FileName == null) {
/*  59 */       throw new NullPointerException("FileName");
/*     */     }
/*  61 */     this.buffer = new short[2304];
/*  62 */     this.bufferp = new short[2];
/*  63 */     this.channels = number_of_channels;
/*     */ 
/*  65 */     for (int i = 0; i < number_of_channels; i++) {
/*  66 */       this.bufferp[i] = (short)i;
/*     */     }
/*  68 */     this.outWave = new WaveFile();
/*     */ 
/*  70 */     int rc = this.outWave.OpenForWrite(FileName, freq, 16, (short)this.channels);
/*     */   }
/*     */ 
/*     */   public void append(int channel, short value)
/*     */   {
/*  78 */     this.buffer[this.bufferp[channel]] = value;
/*     */     int tmp17_16 = channel;
/*     */     short[] tmp17_13 = this.bufferp; tmp17_13[tmp17_16] = (short)(tmp17_13[tmp17_16] + this.channels);
/*     */   }
/*     */ 
/*     */   public void write_buffer(int val)
/*     */   {
/*  89 */     int k = 0;
/*  90 */     int rc = 0;
/*     */ 
/*  92 */     rc = this.outWave.WriteData(this.buffer, this.bufferp[0]);
/*     */ 
/* 105 */     for (int i = 0; i < this.channels; i++) this.bufferp[i] = (short)i;
/*     */   }
/*     */ 
/*     */   public void close()
/*     */   {
/* 110 */     this.outWave.Close();
/*     */   }
/*     */ 
/*     */   public void clear_buffer()
/*     */   {
/*     */   }
/*     */ 
/*     */   public void set_stop_flag()
/*     */   {
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.converter.WaveFileObuffer
 * JD-Core Version:    0.6.0
 */