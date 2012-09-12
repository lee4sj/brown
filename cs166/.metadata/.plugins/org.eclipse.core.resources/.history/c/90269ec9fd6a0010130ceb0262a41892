/*    */ package kj.ui;
/*    */ 
/*    */ import kj.track.KJTrack;
/*    */ import kj.track.KJTrackList;
/*    */ import kj.track.KJTrackProviderChangeEvent;
/*    */ import kj.track.KJTrackProviderChangeListener;
/*    */ 
/*    */ public class KJTrackListTableModel extends KJListModel
/*    */   implements KJTrackProviderChangeListener
/*    */ {
/*    */   private KJTrackList modeTrackList;
/*    */ 
/*    */   public KJTrackListTableModel()
/*    */   {
/*    */   }
/*    */ 
/*    */   public KJTrackListTableModel(KJTrackList pPlayList)
/*    */   {
/* 27 */     this.modeTrackList = pPlayList;
/*    */   }
/*    */ 
/*    */   public KJTrackList getTrackList() {
/* 31 */     return this.modeTrackList;
/*    */   }
/*    */ 
/*    */   public int getRowCount()
/*    */   {
/* 36 */     if (this.modeTrackList != null) {
/* 37 */       return this.modeTrackList.size();
/*    */     }
/*    */ 
/* 40 */     return 0;
/*    */   }
/*    */ 
/*    */   public Object getValueAt(int pRow, int pColumn)
/*    */   {
/* 46 */     if (this.modeTrackList != null) {
/* 47 */       return this.modeTrackList.getTrack(pRow);
/*    */     }
/* 49 */     return null;
/*    */   }
/*    */ 
/*    */   public Class getColumnClass(int pColumn)
/*    */   {
/* 55 */     return KJTrack.class;
/*    */   }
/*    */ 
/*    */   public void setTrackList(KJTrackList pPlayList)
/*    */   {
/* 60 */     if (this.modeTrackList != null) {
/* 61 */       this.modeTrackList.removeListener(this);
/*    */     }
/*    */ 
/* 64 */     this.modeTrackList = pPlayList;
/*    */ 
/* 66 */     if (this.modeTrackList != null) {
/* 67 */       this.modeTrackList.addListener(this);
/*    */     }
/*    */ 
/* 70 */     update();
/*    */   }
/*    */ 
/*    */   public void trackProviderChanged(KJTrackProviderChangeEvent pEvent)
/*    */   {
/* 75 */     update();
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJTrackListTableModel
 * JD-Core Version:    0.6.0
 */