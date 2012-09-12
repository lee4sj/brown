package constants;

public abstract interface CommunicationConstants
{
  public static final int TRANSFER_BUF_SIZE = 2048;
  public static final String CASH_PATH = "C:\\Documents and Settings\\Joel Weinberger.JOEL\\My Documents\\My Music\\iTunes\\iTunes Music\\Johnny Cash\\At Folsom Prison\\01 Folsom Prison Blues.mp3";
  public static final String DESKTOP_PATH = "C:\\Documents and Settings\\Joel Weinberger.JOEL\\Desktop\\";
  public static final String HOST = "localhost";
  public static final int SERVER_PORT = 8000;
  public static final String OPEN_CONNECTION_FLAG = "OPEN_CONNECTION\n";
  public static final String CLOSE_CONNECTION_FLAG = "CLOSE_CONNECTION\n";
  public static final String INVALID_REQUEST_FLAG = "INVALID_REQUEST\n";
  public static final String REQUEST_ACCOUNT_VALIDATION = "ACCOUNT_VALIDATION_REQUEST\n";
  public static final String INVALID_ACCOUNT = "INVALID_ACCOUNT_VALIDATION\n";
  public static final String VALID_ACCOUNT = "ACCOUNT_IS_VALID\n";
  public static final String REQUEST_FILE_FLAG = "FILE_REQUEST\n";
  public static final String BEGIN_SONG_SEND_FLAG = "BEGIN_SONG_SEND\n";
  public static final String REQUEST_SONG_LIST_FLAG = "SONG_LIST_REQUEST\n";
  public static final String MORE_SONGS_FLAG = "MORE_SONGS\n";
  public static final String END_SONG_LIST_FLAG = "END_SONG_LIST\n";
  public static final String VALID_FILE_REQUEST = "VALID_FILE\n";
  public static final String INVALID_FILE_REQUEST = "INVALID_FILE\n";
}

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     constants.CommunicationConstants
 * JD-Core Version:    0.6.0
 */