/*    */ package kj.ui;
/*    */ 
/*    */ import java.awt.BorderLayout;
/*    */ import java.awt.Container;
/*    */ import java.awt.HeadlessException;
/*    */ import java.io.InputStream;
/*    */ import javax.swing.JDialog;
/*    */ import javax.swing.JFrame;
/*    */ import javax.swing.JScrollPane;
/*    */ import javax.swing.JTextArea;
/*    */ 
/*    */ public class KJHelpDocumentation extends JDialog
/*    */ {
/*    */   private JTextArea documentation;
/*    */ 
/*    */   public KJHelpDocumentation(JFrame pParent)
/*    */     throws HeadlessException
/*    */   {
/* 30 */     super(pParent);
/*    */ 
/* 32 */     prepareGUI();
/*    */   }
/*    */ 
/*    */   private void prepareGUI()
/*    */   {
/* 38 */     setTitle("KJ - Documentation");
/* 39 */     setSize(640, 480);
/*    */ 
/* 41 */     getContentPane().setLayout(new BorderLayout());
/*    */ 
/* 43 */     this.documentation = new JTextArea();
/*    */ 
/* 45 */     getContentPane().add(new JScrollPane(this.documentation));
/*    */ 
/* 47 */     setLocation(KJUITools.calculateCenterScreenPosition(getSize()));
/*    */ 
/* 49 */     setVisible(true);
/*    */ 
/* 51 */     initializeValues();
/*    */   }
/*    */ 
/*    */   private void initializeValues()
/*    */   {
/*    */     try
/*    */     {
/* 59 */       byte[] wBuffer = new byte[4096];
/*    */ 
/* 61 */       InputStream wIs = getClass().getClassLoader().getResourceAsStream("kj_documentation.txt");
/*    */ 
/* 63 */       int wRs = 0;
/*    */ 
/* 65 */       while ((wRs = wIs.read(wBuffer)) != -1)
/* 66 */         this.documentation.append(new String(wBuffer, 0, wRs));
/*    */     }
/*    */     catch (Exception pEx)
/*    */     {
/* 70 */       this.documentation.append("- Unable to load documentation.");
/*    */     }
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJHelpDocumentation
 * JD-Core Version:    0.6.0
 */