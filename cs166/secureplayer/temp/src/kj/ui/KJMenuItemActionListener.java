/*    */ package kj.ui;
/*    */ 
/*    */ import java.awt.event.ActionEvent;
/*    */ import java.awt.event.ActionListener;
/*    */ import java.lang.reflect.InvocationTargetException;
/*    */ import java.lang.reflect.Method;
/*    */ import javax.swing.JMenuItem;
/*    */ 
/*    */ public class KJMenuItemActionListener
/*    */   implements ActionListener
/*    */ {
/*    */   private Object actionClass;
/*    */ 
/*    */   public KJMenuItemActionListener(Object pActionClass)
/*    */   {
/* 27 */     this.actionClass = pActionClass;
/*    */   }
/*    */ 
/*    */   public void actionPerformed(ActionEvent pEvent)
/*    */   {
/*    */     try
/*    */     {
/* 36 */       Method wMenuMethod = this.actionClass.getClass().getDeclaredMethod(
/* 37 */         "mi" + pEvent.getActionCommand(), 
/* 38 */         new Class[] { JMenuItem.class });
/*    */ 
/* 40 */       wMenuMethod.setAccessible(true);
/*    */ 
/* 42 */       wMenuMethod.invoke(
/* 43 */         this.actionClass, 
/* 44 */         new Object[] { pEvent.getSource() });
/*    */     }
/*    */     catch (InvocationTargetException pEx) {
/* 47 */       pEx.printStackTrace();
/*    */     } catch (Exception pEx) {
/* 49 */       pEx.printStackTrace();
/*    */     }
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJMenuItemActionListener
 * JD-Core Version:    0.6.0
 */