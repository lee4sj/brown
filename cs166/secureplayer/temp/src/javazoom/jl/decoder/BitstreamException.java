/*    */ package javazoom.jl.decoder;
/*    */ 
/*    */ public class BitstreamException extends JavaLayerException
/*    */   implements BitstreamErrors
/*    */ {
/* 43 */   private int errorcode = 256;
/*    */ 
/*    */   public BitstreamException(String msg, Throwable t)
/*    */   {
/* 47 */     super(msg, t);
/*    */   }
/*    */ 
/*    */   public BitstreamException(int errorcode, Throwable t)
/*    */   {
/* 52 */     this(getErrorString(errorcode), t);
/* 53 */     this.errorcode = errorcode;
/*    */   }
/*    */ 
/*    */   public int getErrorCode()
/*    */   {
/* 58 */     return this.errorcode;
/*    */   }
/*    */ 
/*    */   public static String getErrorString(int errorcode)
/*    */   {
/* 67 */     return "Bitstream errorcode " + Integer.toHexString(errorcode);
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.decoder.BitstreamException
 * JD-Core Version:    0.6.0
 */