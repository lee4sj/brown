/*    */ package kj.ui;
/*    */ 
/*    */ import java.util.ArrayList;
/*    */ import javax.swing.event.TableModelEvent;
/*    */ import javax.swing.event.TableModelListener;
/*    */ import javax.swing.table.TableModel;
/*    */ 
/*    */ public abstract class KJListModel
/*    */   implements TableModel
/*    */ {
/* 25 */   private ArrayList tableModelListeners = new ArrayList();
/*    */ 
/*    */   public int getColumnCount() {
/* 28 */     return 1;
/*    */   }
/*    */ 
/*    */   public boolean isCellEditable(int pRow, int pColumn) {
/* 32 */     return false;
/*    */   }
/*    */   public void setValueAt(Object arg0, int pRow, int pColumn) {
/*    */   }
/*    */ 
/*    */   public Class getColumnClass(int pColumn) {
/* 38 */     return String.class;
/*    */   }
/*    */ 
/*    */   public String getColumnName(int pColumn) {
/* 42 */     return null;
/*    */   }
/*    */ 
/*    */   public void update()
/*    */   {
/* 47 */     TableModelEvent wEvent = new TableModelEvent(this);
/*    */ 
/* 49 */     for (int a = this.tableModelListeners.size() - 1; a >= 0; a--)
/* 50 */       ((TableModelListener)this.tableModelListeners.get(a)).tableChanged(wEvent);
/*    */   }
/*    */ 
/*    */   public void addTableModelListener(TableModelListener pListener)
/*    */   {
/* 56 */     this.tableModelListeners.add(pListener);
/*    */   }
/*    */ 
/*    */   public void removeTableModelListener(TableModelListener pListener) {
/* 60 */     this.tableModelListeners.remove(pListener);
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJListModel
 * JD-Core Version:    0.6.0
 */