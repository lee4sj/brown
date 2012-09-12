/*     */ package kj.track;
/*     */ 
/*     */ import java.io.File;
/*     */ import java.io.IOException;
/*     */ import java.io.InputStream;
/*     */ import java.io.OutputStream;
/*     */ import java.io.PrintStream;
/*     */ import java.security.InvalidAlgorithmParameterException;
/*     */ import java.security.InvalidKeyException;
/*     */ import java.security.NoSuchAlgorithmException;
/*     */ import java.security.spec.AlgorithmParameterSpec;
/*     */ import javax.crypto.Cipher;
/*     */ import javax.crypto.CipherInputStream;
/*     */ import javax.crypto.NoSuchPaddingException;
/*     */ import javax.crypto.SecretKey;
/*     */ import javax.crypto.spec.IvParameterSpec;
/*     */ import kj.audio.KJAudioDataProvider;
/*     */ import kj.xml.KJXmlSerializable;
/*     */ import kj.xml.KJXmlToolkit;
/*     */ import org.w3c.dom.Element;
/*     */ 
/*     */ public abstract class KJTrack
/*     */   implements KJXmlSerializable
/*     */ {
/*     */   public static final int STATE_INVALID = -1;
/*     */   public static final int STATE_NONE = 0;
/*     */   public static final int STATE_PLAYING = 1;
/*     */   public static final int STATE_PLAYED = 2;
/*     */   public static final int STATE_ENDING = 3;
/*     */   private KJTrackList trackList;
/*     */   private File file;
/*  51 */   private boolean allowRequestsToFollow = true;
/*     */   private KJTrackCrossFade crossFade;
/*  55 */   private boolean crossFadingEnabled = true;
/*     */ 
/*  57 */   private float gain = 0.0F;
/*     */   private KJAudioDataProvider adp;
/*     */   private int state;
/*  63 */   private boolean requested = false;
/*     */ 
/*     */   public KJTrack() {
/*     */   }
/*     */ 
/*     */   public KJTrack(KJTrackList pPlayList, File pFile) {
/*  69 */     this.trackList = pPlayList;
/*  70 */     this.file = pFile;
/*     */   }
/*     */ 
/*     */   public boolean allowRequestsToFollow() {
/*  74 */     return (this.trackList.allowRequests()) && (this.allowRequestsToFollow);
/*     */   }
/*     */   protected abstract KJAudioDataProvider createAudioDataProvider();
/*     */ 
/*     */   public abstract long duration();
/*     */ 
/*  82 */   public File file() { return this.file; }
/*     */ 
/*     */   public boolean getAllowRequestsToFollow()
/*     */   {
/*  86 */     return this.allowRequestsToFollow;
/*     */   }
/*     */ 
/*     */   public KJAudioDataProvider getAudioDataProvider() {
/*  90 */     return getAudioDataProvider(false);
/*     */   }
/*     */ 
/*     */   public KJAudioDataProvider getAudioDataProvider(boolean pNewProvider)
/*     */   {
/*  95 */     if ((pNewProvider) || (this.adp == null) || (this.adp.isDisposed())) {
/*  96 */       this.adp = createAudioDataProvider();
/*     */     }
/*     */ 
/*  99 */     return this.adp;
/*     */   }
/*     */ 
/*     */   public KJTrackCrossFade getCrossFade()
/*     */   {
/* 104 */     return this.crossFade;
/*     */   }
/*     */ 
/*     */   public float getGain() {
/* 108 */     return this.gain;
/*     */   }
/*     */ 
/*     */   public KJTrackList getTrackList() {
/* 112 */     return this.trackList;
/*     */   }
/*     */ 
/*     */   public KJTrackCrossFade getResolvedCrossFader()
/*     */   {
/* 117 */     if ((this.trackList.isCrossFadingEnabled()) && (isCrossFadingEnabled()))
/*     */     {
/* 119 */       if (this.crossFade != null) {
/* 120 */         return this.crossFade;
/*     */       }
/*     */ 
/* 123 */       return this.trackList.getCrossFade();
/*     */     }
/*     */ 
/* 127 */     return null;
/*     */   }
/*     */ 
/*     */   public int getState()
/*     */   {
/* 132 */     return this.state;
/*     */   }
/*     */ 
/*     */   public boolean isCrossFadingEnabled() {
/* 136 */     return this.crossFadingEnabled;
/*     */   }
/*     */ 
/*     */   public boolean isRequested() {
/* 140 */     return this.requested;
/*     */   }
/*     */ 
/*     */   public void setAllowRequestsToFollow(boolean pState) {
/* 144 */     this.allowRequestsToFollow = pState;
/*     */   }
/*     */ 
/*     */   public void setCrossFade(KJTrackCrossFade pCrossFade) {
/* 148 */     this.crossFade = pCrossFade;
/*     */   }
/*     */ 
/*     */   public void setCrossFadingEnabled(boolean pState) {
/* 152 */     this.crossFadingEnabled = pState;
/*     */   }
/*     */ 
/*     */   public void setGain(float pGainPercentage) {
/* 156 */     this.gain = pGainPercentage;
/*     */   }
/*     */ 
/*     */   public void setRequested(boolean pState) {
/* 160 */     this.requested = pState;
/*     */   }
/*     */ 
/*     */   public void setState(int pState) {
/* 164 */     this.state = pState;
/*     */   }
/*     */ 
/*     */   void setTrackList(KJTrackList pTrackList) {
/* 168 */     this.trackList = pTrackList;
/*     */   }
/*     */ 
/*     */   public long size() {
/* 172 */     return this.file.length();
/*     */   }
/*     */ 
/*     */   public String title()
/*     */   {
/* 177 */     if (this.file != null) {
/* 178 */       return this.file.getName();
/*     */     }
/*     */ 
/* 181 */     return "<no name>";
/*     */   }
/*     */ 
/*     */   public String toString()
/*     */   {
/* 186 */     return title();
/*     */   }
/*     */ 
/*     */   public void decrypt(InputStream in, OutputStream out, SecretKey key)
/*     */   {
/* 191 */     byte[] iv = new byte[8];
/*     */ 
/* 194 */     AlgorithmParameterSpec paramSpec = new IvParameterSpec(iv);
/*     */ 
/* 196 */     Cipher dcipher = null;
/* 197 */     byte[] buf = new byte[1024];
/*     */     try
/*     */     {
/* 200 */       dcipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
/* 201 */       dcipher.init(2, key, paramSpec);
/*     */     } catch (NoSuchAlgorithmException localNoSuchAlgorithmException) {
/*     */     } catch (NoSuchPaddingException localNoSuchPaddingException) {
/*     */     } catch (InvalidKeyException e) {
/* 205 */       System.out.println("INVALID KEY! Exiting program!");
/* 206 */       e.printStackTrace();
/* 207 */       System.exit(1);
/*     */     }
/*     */     catch (InvalidAlgorithmParameterException e)
/*     */     {
/* 211 */       e.printStackTrace();
/*     */     }
/*     */ 
/*     */     try
/*     */     {
/* 216 */       in = new CipherInputStream(in, dcipher);
/*     */ 
/* 219 */       int numRead = 0;
/* 220 */       while ((numRead = in.read(buf)) >= 0) {
/* 221 */         out.write(buf, 0, numRead);
/*     */       }
/* 223 */       out.close();
/*     */     }
/*     */     catch (IOException localIOException) {
/*     */     }
/*     */   }
/*     */ 
/*     */   public void readXml(Element pSource) {
/* 230 */     this.file = new File(KJXmlToolkit.get(pSource, "File"));
/*     */ 
/* 232 */     this.allowRequestsToFollow = 
/* 233 */       KJXmlToolkit.get(pSource, 
/* 233 */       "AllowRequestsToFollow", true);
/* 234 */     this.gain = KJXmlToolkit.get(pSource, "Gain", 0.0F);
/* 235 */     this.requested = KJXmlToolkit.get(pSource, "Requested", false);
/* 236 */     this.crossFadingEnabled = 
/* 237 */       KJXmlToolkit.get(pSource, "CrossFadingEnabled", 
/* 237 */       true);
/* 238 */     this.state = KJXmlToolkit.get(pSource, "State", 0);
/*     */ 
/* 240 */     this.crossFade = 
/* 241 */       ((KJTrackCrossFade)KJXmlToolkit.getObject(pSource, 
/* 241 */       "CrossFade"));
/*     */   }
/*     */ 
/*     */   public void writeXml(Element pTarget)
/*     */   {
/* 247 */     KJXmlToolkit.set(pTarget, "File", this.file);
/* 248 */     KJXmlToolkit.set(pTarget, "AllowRequestsToFollow", 
/* 249 */       this.allowRequestsToFollow);
/* 250 */     KJXmlToolkit.set(pTarget, "CrossFadingEnabled", this.crossFadingEnabled);
/* 251 */     KJXmlToolkit.set(pTarget, "Gain", this.gain);
/* 252 */     KJXmlToolkit.set(pTarget, "Requested", this.requested);
/*     */ 
/* 255 */     if ((this.state == 2) || (this.state == -1)) {
/* 256 */       KJXmlToolkit.set(pTarget, "State", this.state);
/*     */     }
/*     */ 
/* 259 */     KJXmlToolkit.setObject(pTarget, "CrossFade", this.crossFade);
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.track.KJTrack
 * JD-Core Version:    0.6.0
 */