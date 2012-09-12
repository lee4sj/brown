/*     */ package kj.storeclient;
/*     */ 
/*     */ import constants.CommunicationConstants;
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.Toolkit;
/*     */ import java.awt.event.ActionEvent;
/*     */ import java.awt.event.ActionListener;
/*     */ import java.io.BufferedReader;
/*     */ import java.io.BufferedWriter;
/*     */ import java.io.File;
/*     */ import java.io.FileInputStream;
/*     */ import java.io.FileOutputStream;
/*     */ import java.io.IOException;
/*     */ import java.io.InputStream;
/*     */ import java.io.InputStreamReader;
/*     */ import java.io.OutputStream;
/*     */ import java.io.OutputStreamWriter;
/*     */ import java.io.PrintStream;
/*     */ import java.net.Socket;
/*     */ import java.security.InvalidAlgorithmParameterException;
/*     */ import java.security.InvalidKeyException;
/*     */ import java.security.NoSuchAlgorithmException;
/*     */ import java.security.spec.AlgorithmParameterSpec;
/*     */ import java.security.spec.InvalidKeySpecException;
/*     */ import java.util.ArrayList;
/*     */ import javax.crypto.Cipher;
/*     */ import javax.crypto.CipherOutputStream;
/*     */ import javax.crypto.NoSuchPaddingException;
/*     */ import javax.crypto.SecretKey;
/*     */ import javax.crypto.SecretKeyFactory;
/*     */ import javax.crypto.spec.DESKeySpec;
/*     */ import javax.crypto.spec.IvParameterSpec;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JFrame;
/*     */ import javax.swing.JLabel;
/*     */ import javax.swing.JPanel;
/*     */ import kj.storeclient.ui.KJStoreClientUI;
/*     */ import kj.ui.KJJukeBox;
/*     */ 
/*     */ public class KJStoreClient
/*     */   implements CommunicationConstants
/*     */ {
/*  58 */   private static final String FILE_SEPARATOR = System.getProperty("file.separator");
/*  59 */   private static final String USER_DIRECTORY = System.getProperty("user.dir");
/*     */   private static final String DEFAULT_MUSIC_DIR = "music";
/*  62 */   KJStoreClientUI clientUI_ = null;
/*     */ 
/*  64 */   File musicDir = null;
/*     */   private Socket server_;
/*     */   private ArrayList<StoreTrack> tracks_;
/*     */   private KJJukeBox juke_box;
/*     */   private BufferedReader serverReader_;
/*     */   private BufferedWriter serverWriter_;
/*     */ 
/*     */   private KJStoreClient()
/*     */   {
/*     */   }
/*     */ 
/*     */   public KJStoreClient(KJStoreClientUI ui, Socket socket, KJJukeBox parentFrame)
/*     */   {
/*  91 */     this.clientUI_ = ui;
/*     */ 
/*  93 */     this.tracks_ = new ArrayList();
/*  94 */     this.server_ = socket;
/*     */     try
/*     */     {
/*  97 */       this.serverReader_ = new BufferedReader(new InputStreamReader(this.server_.getInputStream()));
/*  98 */       this.serverWriter_ = new BufferedWriter(new OutputStreamWriter(this.server_.getOutputStream()));
/*     */     } catch (IOException e) {
/* 100 */       e.printStackTrace();
/*     */     }
/*     */ 
/* 104 */     this.juke_box = parentFrame;
/*     */ 
/* 106 */     changeMusicDir(USER_DIRECTORY + FILE_SEPARATOR + "music");
/*     */   }
/*     */ 
/*     */   public ArrayList<StoreTrack> getTracks()
/*     */   {
/* 116 */     return this.tracks_;
/*     */   }
/*     */ 
/*     */   public boolean changeMusicDir(String absolutePath)
/*     */   {
/* 129 */     this.musicDir = new File(absolutePath);
/*     */ 
/* 131 */     if (!this.musicDir.isAbsolute()) {
/* 132 */       System.err.println("isn't absolute");
/* 133 */       this.musicDir = null;
/* 134 */       return false;
/*     */     }
/*     */ 
/* 137 */     if (this.musicDir.isFile()) {
/* 138 */       System.err.println("music directory cannot be a regular file");
/* 139 */       this.musicDir = null;
/* 140 */       return false;
/*     */     }
/*     */ 
/* 144 */     if ((!this.musicDir.exists()) && 
/* 145 */       (!this.musicDir.mkdirs())) {
/* 146 */       System.err.println("invalid path name while changing music directories");
/* 147 */       this.musicDir = null;
/* 148 */       return false;
/*     */     }
/*     */ 
/* 152 */     return true;
/*     */   }
/*     */ 
/*     */   public boolean connect()
/*     */   {
/* 162 */     String response = null;
/* 163 */     File dest = null;
/*     */     try
/*     */     {
/* 180 */       this.serverReader_ = new BufferedReader(new InputStreamReader(this.server_.getInputStream()));
/* 181 */       this.serverWriter_ = new BufferedWriter(new OutputStreamWriter(this.server_.getOutputStream()));
/*     */ 
/* 192 */       refreshSongListFromServer();
/*     */     }
/*     */     catch (IOException ioe)
/*     */     {
/* 197 */       System.err.println("error with writing or reading to/from server");
/* 198 */       ioe.printStackTrace();
/*     */ 
/* 200 */       return false;
/*     */     }
/*     */ 
/* 203 */     return true;
/*     */   }
/*     */ 
/*     */   public void refreshSongListFromServer() throws IOException
/*     */   {
/* 208 */     String response = null;
/*     */ 
/* 210 */     this.serverWriter_.write("SONG_LIST_REQUEST\n");
/* 211 */     this.serverWriter_.flush();
/*     */ 
/* 214 */     this.tracks_.clear();
/*     */ 
/* 216 */     response = this.serverReader_.readLine();
/* 217 */     if (response.equals("INVALID_ACCOUNT_VALIDATION\n")) {
/* 218 */       new MsgBox(this.juke_box, "Invalid account.  Shutting down");
/*     */     }
/*     */ 
/* 221 */     while ("MORE_SONGS\n".equals(response + "\n"))
/*     */     {
/* 223 */       response = this.serverReader_.readLine();
/*     */ 
/* 226 */       this.tracks_.add(new StoreTrack(response, this.musicDir.getPath() + FILE_SEPARATOR + response));
/*     */ 
/* 228 */       response = this.serverReader_.readLine();
/*     */     }
/*     */   }
/*     */ 
/*     */   public boolean isConnected()
/*     */   {
/* 258 */     return (this.server_ != null) && (this.server_.isConnected());
/*     */   }
/*     */ 
/*     */   private void closeServerConnection() throws IOException {
/* 262 */     if (!this.server_.isClosed()) {
/* 263 */       this.serverReader_.close();
/* 264 */       this.serverWriter_.close();
/* 265 */       this.server_.close();
/*     */ 
/* 267 */       this.serverReader_ = null;
/* 268 */       this.serverWriter_ = null;
/* 269 */       this.server_ = null;
/*     */     }
/*     */   }
/*     */ 
/*     */   public String downloadTrack(StoreTrack track)
/*     */   {
/* 282 */     String response = null;
/* 283 */     String path = null;
/* 284 */     String file = track.getPath();
/* 285 */     file = file.substring(0, file.length() - 1).concat("3");
/* 286 */     File trackFile = new File(file);
/*     */     try
/*     */     {
/* 290 */       this.serverWriter_.write("FILE_REQUEST\n");
/* 291 */       this.serverWriter_.flush();
/*     */ 
/* 294 */       this.serverWriter_.write(track.getName() + "\n");
/* 295 */       this.serverWriter_.flush();
/*     */ 
/* 297 */       response = this.serverReader_.readLine();
/*     */ 
/* 301 */       if (response.equals("INVALID_ACCOUNT_VALIDATION\n")) {
/* 302 */         new MsgBox(this.juke_box, "Invalid account.  Shutting down");
/*     */       }
/* 305 */       else if ((response.split(" ")[0] + "\n").equals("VALID_FILE\n")) {
/* 306 */         this.serverWriter_.write("BEGIN_SONG_SEND\n");
/* 307 */         this.serverWriter_.flush();
/* 308 */         long fileLength = Integer.parseInt(response.split(" ")[1]);
/*     */ 
/* 310 */         FileOutputStream outFile = new FileOutputStream(trackFile.getAbsolutePath());
/* 311 */         InputStream inStream = this.server_.getInputStream();
/* 312 */         byte[] buf = new byte[2048];
/* 313 */         long totalBytesRead = 0L;
/*     */ 
/* 318 */         while (totalBytesRead < fileLength) {
/* 319 */           int bytesRead = inStream.read(buf, 0, 2048);
/*     */ 
/* 321 */           outFile.write(buf, 0, bytesRead);
/* 322 */           totalBytesRead += bytesRead;
/*     */         }
/*     */ 
/* 327 */         outFile.flush();
/* 328 */         outFile.close();
/* 329 */         outFile = null;
/*     */ 
/* 331 */         String passkey = this.juke_box.getPassword();
/*     */ 
/* 334 */         if (passkey.length() > 8)
/* 335 */           passkey = passkey.substring(0, 8);
/* 336 */         while (passkey.length() < 8) {
/* 337 */           passkey = passkey.concat("0");
/*     */         }
/* 339 */         byte[] bytes = passkey.getBytes();
/*     */ 
/* 341 */         DESKeySpec specs = null;
/*     */         try {
/* 343 */           specs = new DESKeySpec(bytes);
/*     */         }
/*     */         catch (InvalidKeyException e) {
/* 346 */           e.printStackTrace();
/*     */         }
/* 348 */         SecretKey key = null;
/*     */         try {
/* 350 */           key = SecretKeyFactory.getInstance("DES").generateSecret(specs);
/*     */         }
/*     */         catch (InvalidKeySpecException e) {
/* 353 */           e.printStackTrace();
/*     */         }
/*     */         catch (NoSuchAlgorithmException e) {
/* 356 */           e.printStackTrace();
/*     */         }
/*     */ 
/* 359 */         String oldFile = trackFile.getAbsolutePath();
/*     */ 
/* 361 */         String newFile = oldFile.substring(0, oldFile.length() - 1).concat("S");
/*     */ 
/* 363 */         FileInputStream fis = new FileInputStream(oldFile);
/*     */ 
/* 365 */         encrypt(fis, 
/* 366 */           new FileOutputStream(newFile), key);
/* 367 */         fis.close();
/* 368 */         fis = null;
/* 369 */         trackFile.delete();
/*     */ 
/* 371 */         File newTrackFile = new File(newFile);
/*     */ 
/* 373 */         this.juke_box.addSong(newTrackFile);
/*     */       }
/*     */ 
/*     */     }
/*     */     catch (IOException ioe)
/*     */     {
/* 382 */       System.err.println("Error downloading track");
/* 383 */       ioe.printStackTrace();
/*     */     }
/*     */ 
/* 386 */     return path;
/*     */   }
/*     */ 
   public void encrypt(InputStream in, OutputStream out, SecretKey key)
   {
       /*
     byte[] iv = new byte[8];
 
     AlgorithmParameterSpec paramSpec = new IvParameterSpec(iv);
 
     Cipher ecipher = null;
     byte[] buf = new byte[1024];
     try
     {
       ecipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
       ecipher.init(1, key, paramSpec);
     } catch (NoSuchAlgorithmException localNoSuchAlgorithmException) {
     } catch (NoSuchPaddingException localNoSuchPaddingException) {
     } catch (InvalidKeyException e) {
       System.out.println("INVALID KEY! Exiting program!");
       e.printStackTrace();
       System.exit(1);
     }
     catch (InvalidAlgorithmParameterException e)
     {
       e.printStackTrace();
     }
 
     try
     {
       out = new CipherOutputStream(out, ecipher);
 
       int numRead = 0;
       while ((numRead = in.read(buf)) >= 0) {
         out.write(buf, 0, numRead);
       }
       out.close();
     }
     catch (IOException localIOException)
     {
     }*/
   }
 
/*     */   public class MsgBox extends JDialog
/*     */     implements ActionListener
/*     */   {
/*     */     JButton ok;
/*     */ 
/*     */     public MsgBox(JFrame frame, String msg)
/*     */     {
/* 498 */       super("Authentication failed");
/* 499 */       setLayout(new BorderLayout());
/* 500 */       add("Center", new JLabel(msg));
/* 501 */       addOKCancelPanel();
/* 502 */       createFrame();
/* 503 */       pack();
/* 504 */       setVisible(true);
/*     */     }
/*     */ 
/*     */     void addOKCancelPanel() {
/* 508 */       JPanel p = new JPanel();
/* 509 */       createOKButton(p);
/* 510 */       add("South", p);
/*     */     }
/*     */ 
/*     */     void createOKButton(JPanel p) {
/* 514 */       p.add(this.ok = new JButton("OK"));
/* 515 */       this.ok.addActionListener(this);
/*     */     }
/*     */ 
/*     */     void createFrame() {
/* 519 */       Dimension d = getToolkit().getScreenSize();
/* 520 */       setLocation(d.width / 3, d.height / 3);
/*     */     }
/*     */ 
/*     */     public void actionPerformed(ActionEvent ae) {
/* 524 */       if (ae.getSource() == this.ok) {
/* 525 */         setVisible(false);
/* 526 */         KJStoreClient.this.juke_box.disconnect();
/* 527 */         System.exit(-1);
/*     */       }
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/mod_sec_player/
 * Qualified Name:     kj.storeclient.KJStoreClient
 * JD-Core Version:    0.6.0
 */