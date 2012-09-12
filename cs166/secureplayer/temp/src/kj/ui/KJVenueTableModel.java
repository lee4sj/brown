/*    */ package kj.ui;
/*    */ 
/*    */ import kj.venue.KJVenue;
/*    */ 
/*    */ public class KJVenueTableModel extends KJListModel
/*    */ {
/*    */   private KJVenue modelVenue;
/*    */ 
/*    */   public KJVenueTableModel(KJVenue pVenue)
/*    */   {
/* 23 */     this.modelVenue = pVenue;
/*    */   }
/*    */ 
/*    */   public int getRowCount() {
/* 27 */     return this.modelVenue.getTrackListCount();
/*    */   }
/*    */ 
/*    */   public Object getValueAt(int pRow, int pColumn) {
/* 31 */     return this.modelVenue.getTrackList(pRow);
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJVenueTableModel
 * JD-Core Version:    0.6.0
 */