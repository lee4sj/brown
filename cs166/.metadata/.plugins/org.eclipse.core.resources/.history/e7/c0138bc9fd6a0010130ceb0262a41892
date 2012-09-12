/*    */ package kj.player;
/*    */ 
/*    */ import javax.sound.sampled.SourceDataLine;
/*    */ import kj.audio.KJAudioDataConsumer;
/*    */ 
/*    */ public class KJSourceDataLineAudioDataConsumer
/*    */   implements KJAudioDataConsumer
/*    */ {
/*    */   private SourceDataLine sourceDataLine;
/*    */ 
/*    */   public KJSourceDataLineAudioDataConsumer(SourceDataLine pSdl)
/*    */   {
/* 24 */     this.sourceDataLine = pSdl;
/*    */   }
/*    */ 
/*    */   public void writeAudioData(byte[] pAudioData, boolean encrypted) {
/* 28 */     this.sourceDataLine.write(pAudioData, 0, pAudioData.length);
/*    */   }
/*    */ 
/*    */   public void writeAudioData(byte[] pAudioData, int pOffset, int pLength, boolean encrypted) {
/* 32 */     this.sourceDataLine.write(pAudioData, pOffset, pLength);
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.player.KJSourceDataLineAudioDataConsumer
 * JD-Core Version:    0.6.0
 */