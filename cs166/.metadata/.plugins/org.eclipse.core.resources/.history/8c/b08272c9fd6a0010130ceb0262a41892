/*     */ package javazoom.jl.decoder;
/*     */ 
/*     */ class LayerIDecoder
/*     */   implements FrameDecoder
/*     */ {
/*     */   protected Bitstream stream;
/*     */   protected Header header;
/*     */   protected SynthesisFilter filter1;
/*     */   protected SynthesisFilter filter2;
/*     */   protected Obuffer buffer;
/*     */   protected int which_channels;
/*     */   protected int mode;
/*     */   protected int num_subbands;
/*     */   protected Subband[] subbands;
/*  41 */   protected Crc16 crc = null;
/*     */ 
/*     */   public LayerIDecoder()
/*     */   {
/*  45 */     this.crc = new Crc16();
/*     */   }
/*     */ 
/*     */   public void create(Bitstream stream0, Header header0, SynthesisFilter filtera, SynthesisFilter filterb, Obuffer buffer0, int which_ch0)
/*     */   {
/*  52 */     this.stream = stream0;
/*  53 */     this.header = header0;
/*  54 */     this.filter1 = filtera;
/*  55 */     this.filter2 = filterb;
/*  56 */     this.buffer = buffer0;
/*  57 */     this.which_channels = which_ch0;
/*     */   }
/*     */ 
/*     */   public void decodeFrame()
/*     */   {
/*  66 */     this.num_subbands = this.header.number_of_subbands();
/*  67 */     this.subbands = new Subband[32];
/*  68 */     this.mode = this.header.mode();
/*     */ 
/*  70 */     createSubbands();
/*     */ 
/*  72 */     readAllocation();
/*  73 */     readScaleFactorSelection();
/*     */ 
/*  75 */     if ((this.crc != null) || (this.header.checksum_ok()))
/*     */     {
/*  77 */       readScaleFactors();
/*     */ 
/*  79 */       readSampleData();
/*     */     }
/*     */   }
/*     */ 
/*     */   protected void createSubbands()
/*     */   {
/*  87 */     if (this.mode == 3) {
/*  88 */       for (int i = 0; i < this.num_subbands; i++)
/*  89 */         this.subbands[i] = new SubbandLayer1(i);
/*  90 */     } else if (this.mode == 1)
/*     */     {
/*  92 */       for (int i = 0; i < this.header.intensity_stereo_bound(); i++)
/*  93 */         this.subbands[i] = new SubbandLayer1Stereo(i);
/*  94 */       for (; i < this.num_subbands; i++)
/*  95 */         this.subbands[i] = new SubbandLayer1IntensityStereo(i);
/*     */     }
/*     */     else
/*     */     {
/*  99 */       for (int i = 0; i < this.num_subbands; i++)
/* 100 */         this.subbands[i] = new SubbandLayer1Stereo(i);
/*     */     }
/*     */   }
/*     */ 
/*     */   protected void readAllocation()
/*     */   {
/* 107 */     for (int i = 0; i < this.num_subbands; i++)
/* 108 */       this.subbands[i].read_allocation(this.stream, this.header, this.crc);
/*     */   }
/*     */ 
/*     */   protected void readScaleFactorSelection()
/*     */   {
/*     */   }
/*     */ 
/*     */   protected void readScaleFactors()
/*     */   {
/* 119 */     for (int i = 0; i < this.num_subbands; i++)
/* 120 */       this.subbands[i].read_scalefactor(this.stream, this.header);
/*     */   }
/*     */ 
/*     */   protected void readSampleData()
/*     */   {
/* 125 */     boolean read_ready = false;
/* 126 */     boolean write_ready = false;
/* 127 */     int mode = this.header.mode();
/*     */     do
/*     */     {
/* 131 */       for (int i = 0; i < this.num_subbands; i++)
/* 132 */         read_ready = this.subbands[i].read_sampledata(this.stream);
/*     */       do
/*     */       {
/* 135 */         for (i = 0; i < this.num_subbands; i++) {
/* 136 */           write_ready = this.subbands[i].put_next_sample(this.which_channels, this.filter1, this.filter2);
/*     */         }
/* 138 */         this.filter1.calculate_pcm_samples(this.buffer);
/* 139 */         if ((this.which_channels == 0) && (mode != 3))
/* 140 */           this.filter2.calculate_pcm_samples(this.buffer);
/*     */       }
/* 133 */       while (!
/* 141 */         write_ready);
/*     */     }
/* 129 */     while (!
/* 142 */       read_ready);
/*     */   }
/*     */ 
/*     */   static abstract class Subband
/*     */   {
/* 158 */     public static final float[] scalefactors = { 
/* 159 */       2.0F, 1.587401F, 1.259921F, 1.0F, 
/* 160 */       0.7937005F, 0.6299605F, 0.5F, 0.3968503F, 
/* 161 */       0.3149803F, 0.25F, 0.1984251F, 0.1574901F, 
/* 162 */       0.125F, 0.09921257F, 0.07874507F, 0.0625F, 
/* 163 */       0.04960628F, 0.03937253F, 0.03125F, 0.02480314F, 
/* 164 */       0.01968627F, 0.015625F, 0.01240157F, 0.009843133F, 
/* 165 */       0.007813F, 0.006200785F, 0.004921567F, 0.0039063F, 
/* 166 */       0.003100393F, 0.002460783F, 0.00195313F, 0.001550196F, 
/* 167 */       0.001230392F, 0.000976563F, 0.0007750982F, 0.0006151958F, 
/* 168 */       0.0004882813F, 0.0003875491F, 0.0003075979F, 0.0002441406F, 
/* 169 */       0.0001937745F, 0.000153799F, 0.0001220703F, 9.688727E-05F, 
/* 170 */       7.689948E-05F, 6.103516E-05F, 4.844364E-05F, 3.844974E-05F, 
/* 171 */       3.051758E-05F, 2.422182E-05F, 1.922487E-05F, 1.525879E-05F, 
/* 172 */       1.211091E-05F, 9.612435E-06F, 7.629395E-06F, 6.055454E-06F, 
/* 173 */       4.806218E-06F, 3.814697E-06F, 3.027727E-06F, 2.403109E-06F, 
/* 174 */       1.907349E-06F, 1.513864E-06F, 1.201554E-06F, 0.0F };
/*     */ 
/*     */     public abstract void read_allocation(Bitstream paramBitstream, Header paramHeader, Crc16 paramCrc16);
/*     */ 
/*     */     public abstract void read_scalefactor(Bitstream paramBitstream, Header paramHeader);
/*     */ 
/*     */     public abstract boolean read_sampledata(Bitstream paramBitstream);
/*     */ 
/*     */     public abstract boolean put_next_sample(int paramInt, SynthesisFilter paramSynthesisFilter1, SynthesisFilter paramSynthesisFilter2);
/*     */   }
/* 192 */   static class SubbandLayer1 extends LayerIDecoder.Subband { public static final float[] table_factor = { 
/* 193 */       0.0F, 0.6666667F, 0.285714F, 0.1333333F, 
/* 194 */       0.06451613F, 0.03174603F, 0.01574803F, 
/* 195 */       0.007843138F, 0.003913894F, 
/* 196 */       0.001955034F, 0.0009770396F, 
/* 197 */       0.0004884005F, 0.0002441704F, 
/* 198 */       0.0001220778F, 6.103702E-05F };
/*     */ 
/* 201 */     public static final float[] table_offset = { 
/* 202 */       0.0F, -0.6666667F, -0.8571429F, -0.933333F, 
/* 203 */       -0.9677419F, -0.984127F, -0.992126F, 
/* 204 */       -0.9960785F, -0.9980431F, 
/* 205 */       -0.9990225F, -0.9995115F, 
/* 206 */       -0.9997559F, -0.9998779F, 
/* 207 */       -0.999939F, -0.9999695F };
/*     */     protected int subbandnumber;
/*     */     protected int samplenumber;
/*     */     protected int allocation;
/*     */     protected float scalefactor;
/*     */     protected int samplelength;
/*     */     protected float sample;
/*     */     protected float factor;
/*     */     protected float offset;
/*     */ 
/* 223 */     public SubbandLayer1(int subbandnumber) { this.subbandnumber = subbandnumber;
/* 224 */       this.samplenumber = 0;
/*     */     }
/*     */ 
/*     */     public void read_allocation(Bitstream stream, Header header, Crc16 crc)
/*     */     {
/* 232 */       if (((this.allocation = stream.get_bits(4)) != 15) || 
/* 235 */         (crc != null))
/* 236 */         crc.add_bits(this.allocation, 4);
/* 237 */       if (this.allocation != 0)
/*     */       {
/* 239 */         this.samplelength = (this.allocation + 1);
/* 240 */         this.factor = table_factor[this.allocation];
/* 241 */         this.offset = table_offset[this.allocation];
/*     */       }
/*     */     }
/*     */ 
/*     */     public void read_scalefactor(Bitstream stream, Header header)
/*     */     {
/* 250 */       if (this.allocation != 0) this.scalefactor = scalefactors[stream.get_bits(6)];
/*     */     }
/*     */ 
/*     */     public boolean read_sampledata(Bitstream stream)
/*     */     {
/* 258 */       if (this.allocation != 0)
/*     */       {
/* 260 */         this.sample = stream.get_bits(this.samplelength);
/*     */       }
/* 262 */       if (++this.samplenumber == 12)
/*     */       {
/* 264 */         this.samplenumber = 0;
/* 265 */         return true;
/*     */       }
/* 267 */       return false;
/*     */     }
/*     */ 
/*     */     public boolean put_next_sample(int channels, SynthesisFilter filter1, SynthesisFilter filter2)
/*     */     {
/* 275 */       if ((this.allocation != 0) && (channels != 2))
/*     */       {
/* 277 */         float scaled_sample = (this.sample * this.factor + this.offset) * this.scalefactor;
/* 278 */         filter1.input_sample(scaled_sample, this.subbandnumber);
/*     */       }
/* 280 */       return true;
/*     */     }
/*     */   }
/*     */ 
/*     */   static class SubbandLayer1IntensityStereo extends LayerIDecoder.SubbandLayer1
/*     */   {
/*     */     protected float channel2_scalefactor;
/*     */ 
/*     */     public SubbandLayer1IntensityStereo(int subbandnumber)
/*     */     {
/* 296 */       super();
/*     */     }
/*     */ 
/*     */     public void read_allocation(Bitstream stream, Header header, Crc16 crc)
/*     */     {
/* 304 */       super.read_allocation(stream, header, crc);
/*     */     }
/*     */ 
/*     */     public void read_scalefactor(Bitstream stream, Header header)
/*     */     {
/* 312 */       if (this.allocation != 0)
/*     */       {
/* 314 */         this.scalefactor = scalefactors[stream.get_bits(6)];
/* 315 */         this.channel2_scalefactor = scalefactors[stream.get_bits(6)];
/*     */       }
/*     */     }
/*     */ 
/*     */     public boolean read_sampledata(Bitstream stream)
/*     */     {
/* 324 */       return super.read_sampledata(stream);
/*     */     }
/*     */ 
/*     */     public boolean put_next_sample(int channels, SynthesisFilter filter1, SynthesisFilter filter2)
/*     */     {
/* 332 */       if (this.allocation != 0)
/*     */       {
/* 334 */         this.sample = (this.sample * this.factor + this.offset);
/* 335 */         if (channels == 0)
/*     */         {
/* 337 */           float sample1 = this.sample * this.scalefactor;
/* 338 */           float sample2 = this.sample * this.channel2_scalefactor;
/* 339 */           filter1.input_sample(sample1, this.subbandnumber);
/* 340 */           filter2.input_sample(sample2, this.subbandnumber);
/*     */         }
/* 342 */         else if (channels == 1)
/*     */         {
/* 344 */           float sample1 = this.sample * this.scalefactor;
/* 345 */           filter1.input_sample(sample1, this.subbandnumber);
/*     */         }
/*     */         else
/*     */         {
/* 349 */           float sample2 = this.sample * this.channel2_scalefactor;
/* 350 */           filter1.input_sample(sample2, this.subbandnumber);
/*     */         }
/*     */       }
/* 353 */       return true;
/*     */     }
/*     */   }
/*     */ 
/*     */   static class SubbandLayer1Stereo extends LayerIDecoder.SubbandLayer1
/*     */   {
/*     */     protected int channel2_allocation;
/*     */     protected float channel2_scalefactor;
/*     */     protected int channel2_samplelength;
/*     */     protected float channel2_sample;
/*     */     protected float channel2_factor;
/*     */     protected float channel2_offset;
/*     */ 
/*     */     public SubbandLayer1Stereo(int subbandnumber)
/*     */     {
/* 374 */       super();
/*     */     }
/*     */ 
/*     */     public void read_allocation(Bitstream stream, Header header, Crc16 crc)
/*     */     {
/* 382 */       this.allocation = stream.get_bits(4);
/* 383 */       this.channel2_allocation = stream.get_bits(4);
/* 384 */       if (crc != null)
/*     */       {
/* 386 */         crc.add_bits(this.allocation, 4);
/* 387 */         crc.add_bits(this.channel2_allocation, 4);
/*     */       }
/* 389 */       if (this.allocation != 0)
/*     */       {
/* 391 */         this.samplelength = (this.allocation + 1);
/* 392 */         this.factor = table_factor[this.allocation];
/* 393 */         this.offset = table_offset[this.allocation];
/*     */       }
/* 395 */       if (this.channel2_allocation != 0)
/*     */       {
/* 397 */         this.channel2_samplelength = (this.channel2_allocation + 1);
/* 398 */         this.channel2_factor = table_factor[this.channel2_allocation];
/* 399 */         this.channel2_offset = table_offset[this.channel2_allocation];
/*     */       }
/*     */     }
/*     */ 
/*     */     public void read_scalefactor(Bitstream stream, Header header)
/*     */     {
/* 408 */       if (this.allocation != 0) this.scalefactor = scalefactors[stream.get_bits(6)];
/* 409 */       if (this.channel2_allocation != 0) this.channel2_scalefactor = scalefactors[stream.get_bits(6)];
/*     */     }
/*     */ 
/*     */     public boolean read_sampledata(Bitstream stream)
/*     */     {
/* 417 */       boolean returnvalue = super.read_sampledata(stream);
/* 418 */       if (this.channel2_allocation != 0)
/*     */       {
/* 420 */         this.channel2_sample = stream.get_bits(this.channel2_samplelength);
/*     */       }
/* 422 */       return returnvalue;
/*     */     }
/*     */ 
/*     */     public boolean put_next_sample(int channels, SynthesisFilter filter1, SynthesisFilter filter2)
/*     */     {
/* 430 */       super.put_next_sample(channels, filter1, filter2);
/* 431 */       if ((this.channel2_allocation != 0) && (channels != 1))
/*     */       {
/* 433 */         float sample2 = (this.channel2_sample * this.channel2_factor + this.channel2_offset) * 
/* 434 */           this.channel2_scalefactor;
/* 435 */         if (channels == 0)
/* 436 */           filter2.input_sample(sample2, this.subbandnumber);
/*     */         else
/* 438 */           filter1.input_sample(sample2, this.subbandnumber);
/*     */       }
/* 440 */       return true;
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.decoder.LayerIDecoder
 * JD-Core Version:    0.6.0
 */