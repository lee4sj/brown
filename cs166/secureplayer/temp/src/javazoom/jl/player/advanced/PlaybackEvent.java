/*    */ package javazoom.jl.player.advanced;
/*    */ 
/*    */ public class PlaybackEvent
/*    */ {
/* 28 */   public static int STOPPED = 1;
/* 29 */   public static int STARTED = 2;
/*    */   private AdvancedPlayer source;
/*    */   private int frame;
/*    */   private int id;
/*    */ 
/*    */   public PlaybackEvent(AdvancedPlayer source, int id, int frame)
/*    */   {
/* 37 */     this.id = id;
/* 38 */     this.source = source;
/* 39 */     this.frame = frame;
/*    */   }
/*    */   public int getId() {
/* 42 */     return this.id; } 
/* 43 */   public void setId(int id) { this.id = id; } 
/*    */   public int getFrame() {
/* 45 */     return this.frame; } 
/* 46 */   public void setFrame(int frame) { this.frame = frame; } 
/*    */   public AdvancedPlayer getSource() {
/* 48 */     return this.source; } 
/* 49 */   public void setSource(AdvancedPlayer source) { this.source = source;
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.player.advanced.PlaybackEvent
 * JD-Core Version:    0.6.0
 */