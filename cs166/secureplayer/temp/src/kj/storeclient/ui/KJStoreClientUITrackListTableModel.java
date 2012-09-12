/*     */ package kj.storeclient.ui;
/*     */ 
/*     */ import java.util.ArrayList;
/*     */ import java.util.List;
/*     */ import javax.swing.table.AbstractTableModel;
/*     */ 
/*     */ public class KJStoreClientUITrackListTableModel extends AbstractTableModel
/*     */ {
/*     */   private List table_;
/*     */ 
/*     */   public KJStoreClientUITrackListTableModel()
/*     */   {
/*  23 */     this.table_ = new ArrayList();
/*     */   }
/*     */ 
/*     */   public int getRowCount()
/*     */   {
/*  33 */     return this.table_.size();
/*     */   }
/*     */ 
/*     */   public int getColumnCount()
/*     */   {
/*  43 */     return 1;
/*     */   }
/*     */ 
/*     */   public Object getValueAt(int row, int col)
/*     */   {
/*  53 */     return this.table_.get(row);
/*     */   }
/*     */ 
/*     */   public void setValueAt(Object data, int row, int col)
/*     */   {
/*  66 */     if (row < this.table_.size()) {
/*  67 */       int actualRow = row;
/*  68 */       this.table_.set(row, data);
/*  69 */       fireTableCellUpdated(actualRow, 1);
/*     */     } else {
/*  71 */       int actualRow = this.table_.size();
/*  72 */       this.table_.add(data);
/*  73 */       fireTableRowsInserted(actualRow, 1);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void setTable(List list)
/*     */   {
/*  82 */     this.table_ = list;
/*     */ 
/*  84 */     fireTableDataChanged();
/*     */   }
/*     */ 
/*     */   public boolean removeRow(int firstRow, int lastRow)
/*     */   {
/* 102 */     if ((firstRow < 0) || (lastRow < firstRow) || 
/* 103 */       (firstRow >= this.table_.size()) || (lastRow >= this.table_.size())) {
/* 104 */       return false;
/*     */     }
/*     */ 
/* 108 */     int rowsToMoveUp = lastRow - firstRow + 1;
/*     */ 
/* 110 */     for (int i = firstRow; i <= lastRow; i++) {
/* 111 */       this.table_.remove(i);
/*     */     }
/*     */ 
/* 114 */     fireTableRowsDeleted(firstRow, lastRow);
/*     */ 
/* 116 */     return true;
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.storeclient.ui.KJStoreClientUITrackListTableModel
 * JD-Core Version:    0.6.0
 */