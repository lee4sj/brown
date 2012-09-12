/*    */ package javazoom.jl.decoder;
/*    */ 
/*    */ public class DecoderException extends JavaLayerException
/*    */   implements DecoderErrors
/*    */ {
/* 32 */   private int errorcode = 512;
/*    */ 
/*    */   public DecoderException(String msg, Throwable t)
/*    */   {
/* 36 */     super(msg, t);
/*    */   }
/*    */ 
/*    */   public DecoderException(int errorcode, Throwable t)
/*    */   {
/* 41 */     this(getErrorString(errorcode), t);
/* 42 */     this.errorcode = errorcode;
/*    */   }
/*    */ 
/*    */   public int getErrorCode()
/*    */   {
/* 47 */     return this.errorcode;
/*    */   }
/*    */ 
/*    */   public static String getErrorString(int errorcode)
/*    */   {
/* 56 */     return "Decoder errorcode " + Integer.toHexString(errorcode);
/*    */   }
/*    */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.decoder.DecoderException
 * JD-Core Version:    0.6.0
 */