/*     */ package kj.ui;
/*     */ 
/*     */ import constants.CommunicationConstants;
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Container;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.Point;
/*     */ import java.awt.Toolkit;
/*     */ import java.awt.event.ActionEvent;
/*     */ import java.awt.event.ActionListener;
/*     */ import java.awt.event.WindowAdapter;
/*     */ import java.awt.event.WindowEvent;
/*     */ import java.io.BufferedReader;
/*     */ import java.io.BufferedWriter;
/*     */ import java.io.IOException;
/*     */ import java.io.InputStreamReader;
/*     */ import java.io.OutputStreamWriter;
/*     */ import java.net.Socket;
/*     */ import javax.swing.BoxLayout;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JFrame;
/*     */ import javax.swing.JLabel;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JPasswordField;
/*     */ import javax.swing.JTextField;
/*     */ 
/*     */ public class Authenticator extends JDialog
/*     */   implements CommunicationConstants
/*     */ {
/*     */   private String username;
/*     */   private String password;
/*     */   private KJJukeBox juke_box;
/*     */   private boolean ready_;
/*     */   private BufferedReader serverReader_;
/*     */   private BufferedWriter serverWriter_;
/*     */   private JTextField textUser_;
/*     */   private JTextField textServer_;
/*     */   private JPasswordField textPass_;
/*     */   private Thread threadToInterrupt_;
/*     */ 
/*     */   public Authenticator(KJJukeBox parent, Thread t)
/*     */   {
/*  53 */     super(parent);
/*  54 */     this.juke_box = parent;
/*     */ 
/*  56 */     addWindowListener(new WindowAdapter() {
/*     */       public void windowClosing(WindowEvent event) {
/*  58 */         if (Authenticator.this.username == null)
/*  59 */           System.exit(-1);
/*     */       }
/*     */     });
/*  62 */     prepareGUI();
/*     */ 
/*  64 */     this.threadToInterrupt_ = t;
/*     */   }
/*     */ 
/*     */   private void prepareGUI()
/*     */   {
/*  69 */     setTitle("Checking Access");
/*  70 */     setSize(275, 250);
/*     */ 
/*  72 */     setDefaultCloseOperation(1);
/*     */ 
/*  74 */     Point p = KJUITools.calculateCenterScreenPosition(getSize());
/*  75 */     setLocation(p.x, p.y);
/*     */ 
/*  77 */     JPanel centerPanel = new JPanel();
/*  78 */     centerPanel.setLayout(new BoxLayout(centerPanel, 1));
/*  79 */     getContentPane().add(centerPanel, "Center");
/*     */ 
/*  81 */     JPanel ipPanel = new JPanel();
/*  82 */     JLabel server = new JLabel("Server:");
/*  83 */     this.textServer_ = new JTextField(20);
/*  84 */     ipPanel.add(server);
/*  85 */     ipPanel.add(this.textServer_);
/*  86 */     centerPanel.add(ipPanel);
/*     */ 
/*  88 */     JPanel userPanel = new JPanel();
/*  89 */     JLabel user = new JLabel("Username:");
/*  90 */     this.textUser_ = new JTextField(20);
/*  91 */     userPanel.add(user);
/*  92 */     userPanel.add(this.textUser_);
/*  93 */     centerPanel.add(userPanel);
/*     */ 
/*  95 */     JPanel passPanel = new JPanel();
/*  96 */     JLabel pass = new JLabel("Password:");
/*  97 */     this.textPass_ = new JPasswordField(20);
/*  98 */     passPanel.add(pass);
/*  99 */     passPanel.add(this.textPass_);
/* 100 */     centerPanel.add(passPanel);
/*     */ 
/* 102 */     JPanel buttonPanel = new JPanel();
/* 103 */     JButton connectButton = new JButton("Connect");
/* 104 */     connectButton.addActionListener(new ConnectAction(null));
/* 105 */     JButton cancelButton = new JButton("Cancel");
/* 106 */     cancelButton.addActionListener(new CancelAction(null));
/* 107 */     buttonPanel.add(connectButton);
/* 108 */     buttonPanel.add(cancelButton);
/* 109 */     centerPanel.add(buttonPanel);
/*     */ 
/* 111 */     setVisible(true);
/*     */   }
/*     */ 
/*     */   public String getUser()
/*     */   {
/* 116 */     return this.username;
/*     */   }
/*     */ 
/*     */   public String getPass() {
/* 120 */     return this.password;
/*     */   }
/*     */ 
/*     */   public boolean isReady()
/*     */   {
/* 183 */     return this.ready_;
/*     */   }
/*     */ 
/*     */   private class CancelAction
/*     */     implements ActionListener
/*     */   {
/*     */     private CancelAction()
/*     */     {
/*     */     }
/*     */ 
/*     */     public void actionPerformed(ActionEvent event)
/*     */     {
/* 130 */       Authenticator.this.juke_box.disconnect();
/*     */ 
/* 132 */       System.exit(0);
/*     */     }
/*     */   }
/*     */ 
/*     */   private class ConnectAction implements ActionListener
/*     */   {
/*     */     private ConnectAction()
/*     */     {
/*     */     }
/*     */ 
/*     */     public void actionPerformed(ActionEvent event) {
/* 143 */       Authenticator.this.juke_box.connectToServer(Authenticator.this.textServer_.getText());
/*     */       try
/*     */       {
/* 146 */         Authenticator.this.serverReader_ = 
/* 147 */           new BufferedReader(new InputStreamReader(Authenticator.this.juke_box.getSocket().getInputStream()));
/* 148 */         Authenticator.this.serverWriter_ = 
/* 149 */           new BufferedWriter(new OutputStreamWriter(Authenticator.this.juke_box.getSocket().getOutputStream()));
/*     */       } catch (IOException e) {
/* 151 */         e.printStackTrace();
/*     */       }
/*     */ 
/*     */       try
/*     */       {
/* 156 */         Authenticator.this.serverWriter_.write("ACCOUNT_VALIDATION_REQUEST\n");
/* 157 */         Authenticator.this.serverWriter_.flush();
/* 158 */         Authenticator.this.serverWriter_.write(Authenticator.this.textUser_.getText() + "\n");
/* 159 */         Authenticator.this.serverWriter_.flush();
/* 160 */         Authenticator.this.serverWriter_.write(String.valueOf(Authenticator.this.textPass_.getPassword()) + 
/* 161 */           "\n");
/* 162 */         Authenticator.this.serverWriter_.flush();
/*     */ 
/* 164 */         String response = Authenticator.this.serverReader_.readLine() + "\n";
/*     */ 
/* 166 */         if (response.compareTo("INVALID_ACCOUNT_VALIDATION\n") == 0) {
/* 167 */           new Authenticator.MsgBox(Authenticator.this, Authenticator.this.juke_box, "Invalid account.  Shutting down");
/*     */         } else {
/* 169 */           Authenticator.this.ready_ = true;
/* 170 */           Authenticator.this.threadToInterrupt_.interrupt();
/* 171 */           Authenticator.this.username = Authenticator.this.textUser_.getText();
/* 172 */           Authenticator.this.password = String.valueOf(Authenticator.this.textPass_.getPassword());
/* 173 */           Authenticator.this.setVisible(false);
/*     */         }
/*     */       }
/*     */       catch (IOException localIOException1)
/*     */       {
/*     */       }
/*     */     }
/*     */   }
/*     */ 
/*     */   public class MsgBox extends JDialog
/*     */     implements ActionListener
/*     */   {
/*     */     JButton ok;
/*     */ 
/*     */     public MsgBox(JFrame frame, String msg)
/*     */     {
/* 190 */       super("Authentication failed");
/* 191 */       setLayout(new BorderLayout());
/* 192 */       add("Center", new JLabel(msg));
/* 193 */       addOKCancelPanel();
/* 194 */       createFrame();
/* 195 */       pack();
/* 196 */       setVisible(true);
/*     */     }
/*     */ 
/*     */     void addOKCancelPanel() {
/* 200 */       JPanel p = new JPanel();
/* 201 */       createOKButton(p);
/* 202 */       add("South", p);
/*     */     }
/*     */ 
/*     */     void createOKButton(JPanel p) {
/* 206 */       p.add(this.ok = new JButton("OK"));
/* 207 */       this.ok.addActionListener(this);
/*     */     }
/*     */ 
/*     */     void createFrame() {
/* 211 */       Dimension d = getToolkit().getScreenSize();
/* 212 */       setLocation(d.width / 3, d.height / 3);
/*     */     }
/*     */ 
/*     */     public void actionPerformed(ActionEvent ae) {
/* 216 */       if (ae.getSource() == this.ok) {
/* 217 */         setVisible(false);
/* 218 */         Authenticator.this.juke_box.disconnect();
/* 219 */         System.exit(-1);
/*     */       }
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.Authenticator
 * JD-Core Version:    0.6.0
 */