/*    */ package kj.player;
/*    */ 
/*    */ import java.util.EventObject;
/*    */ import kj.track.KJTrack;
/*    */ 
/*    */ public class KJPlayerEvent extends EventObject
/*    */ {
/*    */   public static final int TYPE_PLAYING = 1;
/*    */   public static final int TYPE_PAUSED = 2;
/*    */   public static final int TYPE_STOPPED = 3;
/*    */   public static final int TYPE_CROSS_FADING = 4;
/*    */   public static final int TYPE_ENDED = 5;
/*    */   public static final int TYPE_NEXT_TRACK = 6;
/*    */   private int type;
/*    */   private KJTrack track;
/*    */ 
/*    */   public KJPlayerEvent(KJPlayer pSource, int pType, KJTrack pTrack)
/*    */   {
/* 33 */     super(pSource);
/*    */ 
/* 35 */     this.type = pType;
/* 36 */     this.track = pTrack;
/*    */   }
/*    */ 
/*    */   public KJPlayer getSourceAsPlayer()
/*    */   {
/* 41 */     return (KJPlayer)getSource();
/*    */   }
/*    */ 
/*    */   public KJTrack getTrack() {
/* 45 */     return this.track;
/*    */   }
/*    */ 
/*    */   public int getType() {
/* 49 */     return this.type;
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.player.KJPlayerEvent
 * JD-Core Version:    0.6.0
 */