/*     */ package javazoom.jl.player;
/*     */ 
/*     */ import javax.sound.sampled.AudioFormat;
/*     */ import javax.sound.sampled.AudioSystem;
/*     */ import javax.sound.sampled.DataLine.Info;
/*     */ import javax.sound.sampled.Line;
/*     */ import javax.sound.sampled.LineUnavailableException;
/*     */ import javax.sound.sampled.SourceDataLine;
/*     */ import javazoom.jl.decoder.Decoder;
/*     */ import javazoom.jl.decoder.JavaLayerException;
/*     */ 
/*     */ public class JavaSoundAudioDevice extends AudioDeviceBase
/*     */ {
/*  46 */   private SourceDataLine source = null;
/*     */ 
/*  48 */   private AudioFormat fmt = null;
/*     */ 
/*  50 */   private byte[] byteBuf = new byte[4096];
/*     */ 
/*     */   protected void setAudioFormat(AudioFormat fmt0)
/*     */   {
/*  54 */     this.fmt = fmt0;
/*     */   }
/*     */ 
/*     */   protected AudioFormat getAudioFormat()
/*     */   {
/*  59 */     if (this.fmt == null)
/*     */     {
/*  61 */       Decoder decoder = getDecoder();
/*  62 */       this.fmt = 
/*  66 */         new AudioFormat(decoder.getOutputFrequency(), 
/*  63 */         16, 
/*  64 */         decoder.getOutputChannels(), 
/*  65 */         true, 
/*  66 */         false);
/*     */     }
/*  68 */     return this.fmt;
/*     */   }
/*     */ 
/*     */   protected DataLine.Info getSourceLineInfo()
/*     */   {
/*  73 */     AudioFormat fmt = getAudioFormat();
/*     */ 
/*  75 */     DataLine.Info info = new DataLine.Info(SourceDataLine.class, fmt);
/*  76 */     return info;
/*     */   }
/*     */ 
/*     */   public void open(AudioFormat fmt) throws JavaLayerException
/*     */   {
/*  81 */     if (!isOpen())
/*     */     {
/*  83 */       setAudioFormat(fmt);
/*  84 */       openImpl();
/*  85 */       setOpen(true);
/*     */     }
/*     */   }
/*     */ 
/*     */   protected void openImpl()
/*     */     throws JavaLayerException
/*     */   {
/*     */   }
/*     */ 
/*     */   protected void createSource()
/*     */     throws JavaLayerException
/*     */   {
/*  98 */     Throwable t = null;
/*     */     try
/*     */     {
/* 101 */       Line line = AudioSystem.getLine(getSourceLineInfo());
/* 102 */       if ((line instanceof SourceDataLine))
/*     */       {
/* 104 */         this.source = ((SourceDataLine)line);
/*     */ 
/* 106 */         this.source.open(this.fmt);
/*     */ 
/* 113 */         this.source.start();
/*     */       }
/*     */     }
/*     */     catch (RuntimeException ex)
/*     */     {
/* 118 */       t = ex;
/*     */     }
/*     */     catch (LinkageError ex)
/*     */     {
/* 122 */       t = ex;
/*     */     }
/*     */     catch (LineUnavailableException ex)
/*     */     {
/* 126 */       t = ex;
/*     */     }
/* 128 */     if (this.source == null) throw new JavaLayerException("cannot obtain source audio line", t);
/*     */   }
/*     */ 
/*     */   public int millisecondsToBytes(AudioFormat fmt, int time)
/*     */   {
/* 133 */     return (int)(time * (fmt.getSampleRate() * fmt.getChannels() * fmt.getSampleSizeInBits()) / 8000.0D);
/*     */   }
/*     */ 
/*     */   protected void closeImpl()
/*     */   {
/* 138 */     if (this.source != null)
/*     */     {
/* 140 */       this.source.close();
/*     */     }
/*     */   }
/*     */ 
/*     */   protected void writeImpl(short[] samples, int offs, int len)
/*     */     throws JavaLayerException
/*     */   {
/* 147 */     if (this.source == null) {
/* 148 */       createSource();
/*     */     }
/* 150 */     byte[] b = toByteArray(samples, offs, len);
/* 151 */     this.source.write(b, 0, len * 2);
/*     */   }
/*     */ 
/*     */   protected byte[] getByteArray(int length)
/*     */   {
/* 156 */     if (this.byteBuf.length < length)
/*     */     {
/* 158 */       this.byteBuf = new byte[length + 1024];
/*     */     }
/* 160 */     return this.byteBuf;
/*     */   }
/*     */ 
/*     */   protected byte[] toByteArray(short[] samples, int offs, int len)
/*     */   {
/* 165 */     byte[] b = getByteArray(len * 2);
/* 166 */     int idx = 0;
/*     */ 
/* 168 */     while (len-- > 0)
/*     */     {
/* 170 */       short s = samples[(offs++)];
/* 171 */       b[(idx++)] = (byte)s;
/* 172 */       b[(idx++)] = (byte)(s >>> 8);
/*     */     }
/* 174 */     return b;
/*     */   }
/*     */ 
/*     */   protected void flushImpl()
/*     */   {
/* 179 */     if (this.source != null)
/*     */     {
/* 181 */       this.source.drain();
/*     */     }
/*     */   }
/*     */ 
/*     */   public int getPosition()
/*     */   {
/* 187 */     int pos = 0;
/* 188 */     if (this.source != null)
/*     */     {
/* 190 */       pos = (int)(this.source.getMicrosecondPosition() / 1000L);
/*     */     }
/* 192 */     return pos;
/*     */   }
/*     */ 
/*     */   public void test()
/*     */     throws JavaLayerException
/*     */   {
/*     */     try
/*     */     {
/* 203 */       open(new AudioFormat(22050.0F, 16, 1, true, false));
/* 204 */       short[] data = new short[2205];
/* 205 */       write(data, 0, data.length);
/* 206 */       flush();
/* 207 */       close();
/*     */     }
/*     */     catch (RuntimeException ex)
/*     */     {
/* 211 */       throw new JavaLayerException("Device test failed: " + ex);
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.player.JavaSoundAudioDevice
 * JD-Core Version:    0.6.0
 */