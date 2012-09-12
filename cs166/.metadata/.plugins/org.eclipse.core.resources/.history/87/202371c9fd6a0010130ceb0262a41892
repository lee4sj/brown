/*    */ package javazoom.jl.decoder;
/*    */ 
/*    */ import java.io.PrintStream;
/*    */ 
/*    */ public class JavaLayerException extends Exception
/*    */ {
/*    */   private Throwable exception;
/*    */ 
/*    */   public JavaLayerException()
/*    */   {
/*    */   }
/*    */ 
/*    */   public JavaLayerException(String msg)
/*    */   {
/* 47 */     super(msg);
/*    */   }
/*    */ 
/*    */   public JavaLayerException(String msg, Throwable t)
/*    */   {
/* 52 */     super(msg);
/* 53 */     this.exception = t;
/*    */   }
/*    */ 
/*    */   public Throwable getException()
/*    */   {
/* 58 */     return this.exception;
/*    */   }
/*    */ 
/*    */   public void printStackTrace()
/*    */   {
/* 64 */     printStackTrace(System.err);
/*    */   }
/*    */ 
/*    */   public void printStackTrace(PrintStream ps)
/*    */   {
/* 69 */     if (this.exception == null)
/*    */     {
/* 71 */       super.printStackTrace(ps);
/*    */     }
/*    */     else
/*    */     {
/* 75 */       this.exception.printStackTrace();
/*    */     }
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.decoder.JavaLayerException
 * JD-Core Version:    0.6.0
 */