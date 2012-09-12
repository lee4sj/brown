/*    */ package kj.track;
/*    */ 
/*    */ import java.util.ArrayList;
/*    */ 
/*    */ public abstract class KJAbstractTrackProvider
/*    */   implements KJTrackProvider
/*    */ {
/* 20 */   private ArrayList listeners = new ArrayList();
/*    */ 
/*    */   public void addListener(KJTrackProviderChangeListener pListener) {
/* 23 */     this.listeners.add(pListener);
/*    */   }
/*    */ 
/*    */   protected void fireTrackProviderChangedEvent() {
/* 27 */     fireTrackProviderChangeEvent(0, null);
/*    */   }
/*    */ 
/*    */   protected void fireTrackProviderChangeEvent(int pType, KJTrack[] pTracks)
/*    */   {
/* 32 */     KJTrackProviderChangeEvent wEvent = new KJTrackProviderChangeEvent(
/* 33 */       this, 
/* 34 */       pType, 
/* 35 */       pTracks);
/*    */ 
/* 37 */     for (int a = 0; a < this.listeners.size(); a++)
/* 38 */       ((KJTrackProviderChangeListener)this.listeners.get(a)).trackProviderChanged(wEvent);
/*    */   }
/*    */ 
/*    */   protected void fireTrackProviderPropertyChangeEvent(String pPropertyName, boolean pPropertyValue)
/*    */   {
/* 44 */     fireTrackProviderPropertyChangeEvent(pPropertyName, pPropertyValue ? Boolean.TRUE : Boolean.FALSE);
/*    */   }
/*    */ 
/*    */   protected void fireTrackProviderPropertyChangeEvent(String pPropertyName, float pPropertyValue) {
/* 48 */     fireTrackProviderPropertyChangeEvent(pPropertyName, new Float(pPropertyValue));
/*    */   }
/*    */ 
/*    */   protected void fireTrackProviderPropertyChangeEvent(String pPropertyName, int pPropertyValue) {
/* 52 */     fireTrackProviderPropertyChangeEvent(pPropertyName, new Integer(pPropertyValue));
/*    */   }
/*    */ 
/*    */   protected void fireTrackProviderPropertyChangeEvent(String pPropertyName, Object pPropertyValue)
/*    */   {
/* 57 */     KJTrackProviderChangeEvent wEvent = new KJTrackProviderChangeEvent(
/* 58 */       this, 
/* 59 */       pPropertyName, 
/* 60 */       pPropertyValue);
/*    */ 
/* 62 */     for (int a = 0; a < this.listeners.size(); a++)
/* 63 */       ((KJTrackProviderChangeListener)this.listeners.get(a)).trackProviderChanged(wEvent);
/*    */   }
/*    */ 
/*    */   public void removeListener(KJTrackProviderChangeListener pListener)
/*    */   {
/* 69 */     this.listeners.remove(pListener);
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.track.KJAbstractTrackProvider
 * JD-Core Version:    0.6.0
 */