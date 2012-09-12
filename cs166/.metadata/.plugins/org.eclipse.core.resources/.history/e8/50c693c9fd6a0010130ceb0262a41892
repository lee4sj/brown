/*    */ package kj.track;
/*    */ 
/*    */ import java.util.EventObject;
/*    */ 
/*    */ public class KJTrackProviderChangeEvent extends EventObject
/*    */ {
/*    */   public static final int TRACK_PROVIDER_CHANGED = 0;
/*    */   public static final int TRACKS_ADDED = 1;
/*    */   public static final int TRACKS_REMOVED = 2;
/*    */   public static final int TRACKS_CHANGED = 3;
/*    */   public static final int PROPERTY_CHANGED = 4;
/* 27 */   private int type = 0;
/*    */   private KJTrack[] tracks;
/*    */   private String propertyName;
/*    */   private Object propertyValue;
/*    */ 
/*    */   public KJTrackProviderChangeEvent(KJTrackProvider pSource)
/*    */   {
/* 36 */     super(pSource);
/*    */   }
/*    */ 
/*    */   public KJTrackProviderChangeEvent(KJTrackProvider pSource, int pType, KJTrack[] pTracks)
/*    */   {
/* 41 */     super(pSource);
/*    */ 
/* 43 */     this.type = pType;
/*    */ 
/* 45 */     this.tracks = pTracks;
/*    */   }
/*    */ 
/*    */   public KJTrackProviderChangeEvent(KJTrackProvider pSource, String pPropertyName, Object pPropertyValue)
/*    */   {
/* 51 */     super(pSource);
/*    */ 
/* 53 */     this.type = 4;
/*    */ 
/* 55 */     this.propertyName = pPropertyName;
/* 56 */     this.propertyValue = pPropertyValue;
/*    */   }
/*    */ 
/*    */   public String getPropertyName()
/*    */   {
/* 61 */     return this.propertyName;
/*    */   }
/*    */ 
/*    */   public Object getPropertyValue() {
/* 65 */     return this.propertyValue;
/*    */   }
/*    */ 
/*    */   public KJTrack[] getTracks() {
/* 69 */     return this.tracks;
/*    */   }
/*    */ 
/*    */   public int getType() {
/* 73 */     return this.type;
/*    */   }
/*    */ 
/*    */   public boolean isPropertyChange() {
/* 77 */     return this.type == 4;
/*    */   }
/*    */ 
/*    */   public String toString() {
/* 81 */     return super.toString() + " type: " + this.type;
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.track.KJTrackProviderChangeEvent
 * JD-Core Version:    0.6.0
 */