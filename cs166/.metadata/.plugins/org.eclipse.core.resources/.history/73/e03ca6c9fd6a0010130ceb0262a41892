/*     */ package kj.xml;
/*     */ 
/*     */ import com.sun.org.apache.xerces.internal.dom.DOMImplementationImpl;
/*     */ import com.sun.org.apache.xerces.internal.dom.DOMInputImpl;
/*     */ import com.sun.org.apache.xerces.internal.parsers.DOMParserImpl;
/*     */ import com.sun.org.apache.xerces.internal.parsers.XML11Configuration;
/*     */ import com.sun.org.apache.xml.internal.serialize.OutputFormat;
/*     */ import com.sun.org.apache.xml.internal.serialize.XML11Serializer;
/*     */ import com.sun.org.apache.xml.internal.serialize.XMLSerializer;
/*     */ import java.io.IOException;
/*     */ import java.io.InputStream;
/*     */ import java.io.OutputStream;
/*     */ import java.io.StringWriter;
/*     */ import java.util.Iterator;
/*     */ import org.w3c.dom.DOMImplementation;
/*     */ import org.w3c.dom.Document;
/*     */ import org.w3c.dom.Element;
/*     */ import org.w3c.dom.Node;
/*     */ import org.w3c.dom.ls.LSInput;
/*     */ import org.w3c.dom.ls.LSParser;
/*     */ import org.w3c.dom.traversal.DocumentTraversal;
/*     */ import org.w3c.dom.traversal.TreeWalker;
/*     */ 
/*     */ public class KJXmlToolkit
/*     */ {
/*  39 */   private static final LSParser parser = new DOMParserImpl(new XML11Configuration());
/*     */ 
/*     */   public static Element add(Element pDestination, String pName) {
/*  42 */     return (Element)pDestination.appendChild(pDestination.getOwnerDocument().createElement(pName));
/*     */   }
/*     */ 
/*     */   private static KJXmlSerializable createTargetFor(Element pTargetElement)
/*     */   {
/*  47 */     String wClassName = pTargetElement.getAttribute("class");
/*     */ 
/*  49 */     if ((wClassName != null) && (wClassName.length() > 0))
/*     */     {
/*     */       try
/*     */       {
/*  53 */         Class wClass = KJXmlToolkit.class.getClassLoader().loadClass(wClassName);
/*     */ 
/*  55 */         return (KJXmlSerializable)wClass.newInstance();
/*     */       }
/*     */       catch (Exception pEx) {
/*  58 */         throw new RuntimeException(pEx);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/*  63 */     throw new KJXmlSerializableException("No 'Class' element defined for target: " + pTargetElement);
/*     */   }
/*     */ 
/*     */   public static Element find(Element pSource, String pName)
/*     */   {
/*  69 */     Iterator wChildren = getElements(pSource);
/*     */ 
/*  71 */     while (wChildren.hasNext())
/*     */     {
/*  73 */       Element wChild = (Element)wChildren.next();
/*     */ 
/*  75 */       if (wChild.getNodeName().equals(pName)) {
/*  76 */         return wChild;
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/*  81 */     return null;
/*     */   }
/*     */ 
/*     */   public static Iterator getElements(Element pSource)
/*     */   {
/* 103 */     if (pSource != null)
/*     */     {
/* 105 */       TreeWalker wTw = ((DocumentTraversal)pSource.getOwnerDocument()).createTreeWalker(
/* 106 */         pSource, 
/* 107 */         1, 
/* 108 */         null, 
/* 109 */         false);
/*     */ 
/* 111 */       return new ElementIterator(wTw);
/*     */     }
/*     */ 
/* 115 */     return null;
/*     */   }
/*     */ 
/*     */   public static Iterator getElementsFor(Element pSource, String pElementName)
/*     */   {
/* 120 */     return getElements(find(pSource, pElementName));
/*     */   }
/*     */ 
/*     */   public static String get(Element pSource, String pName) {
/* 124 */     return get(pSource, pName, null);
/*     */   }
/*     */ 
/*     */   public static String get(Element pSource, String pName, String pDefault)
/*     */   {
/* 129 */     Element wElement = find(pSource, pName);
/*     */ 
/* 131 */     if (wElement != null) {
/* 132 */       return wElement.getTextContent();
/*     */     }
/*     */ 
/* 135 */     return pDefault;
/*     */   }
/*     */ 
/*     */   public static boolean get(Element pSource, String pName, boolean pDefault)
/*     */   {
/*     */     try
/*     */     {
/* 142 */       return Boolean.parseBoolean(get(pSource, pName)); } catch (Exception pEx) {
/*     */     }
/* 144 */     return pDefault;
/*     */   }
/*     */ 
/*     */   public static double get(Element pSource, String pName, double pDefault)
/*     */   {
/*     */     try
/*     */     {
/* 152 */       return Double.parseDouble(get(pSource, pName)); } catch (Exception pEx) {
/*     */     }
/* 154 */     return pDefault;
/*     */   }
/*     */ 
/*     */   public static float get(Element pSource, String pName, float pDefault)
/*     */   {
/*     */     try
/*     */     {
/* 162 */       return Float.parseFloat(get(pSource, pName)); } catch (Exception pEx) {
/*     */     }
/* 164 */     return pDefault;
/*     */   }
/*     */ 
/*     */   public static int get(Element pSource, String pName, int pDefault)
/*     */   {
/*     */     try
/*     */     {
/* 172 */       return Integer.parseInt(get(pSource, pName)); } catch (Exception pEx) {
/*     */     }
/* 174 */     return pDefault;
/*     */   }
/*     */ 
/*     */   public static long get(Element pSource, String pName, long pDefault)
/*     */   {
/*     */     try
/*     */     {
/* 182 */       return Long.parseLong(get(pSource, pName)); } catch (Exception pEx) {
/*     */     }
/* 184 */     return pDefault;
/*     */   }
/*     */ 
/*     */   public static Object getObject(Element pSource, String pName)
/*     */   {
/* 191 */     Element wTe = find(pSource, pName);
/*     */ 
/* 193 */     return toObject(wTe);
/*     */   }
/*     */ 
/*     */   public static Object getObject(Element pSource, String pName, KJXmlSerializable pTarget)
/*     */   {
/* 199 */     Element wTe = find(pSource, pName);
/*     */ 
/* 201 */     pTarget.readXml(wTe);
/*     */ 
/* 203 */     return pTarget;
/*     */   }
/*     */ 
/*     */   public static Document newDocument(String pName)
/*     */   {
/* 209 */     return DOMImplementationImpl.getDOMImplementation().createDocument(
/* 210 */       null, 
/* 211 */       pName, 
/* 212 */       DOMImplementationImpl.getDOMImplementation().createDocumentType(pName, null, null));
/*     */   }
/*     */ 
/*     */   public static Document loadDocument(InputStream pIs)
/*     */     throws Exception
/*     */   {
/* 218 */     LSInput wInput = new DOMInputImpl();
/*     */ 
/* 220 */     wInput.setByteStream(pIs);
/*     */ 
/* 222 */     return parser.parse(wInput);
/*     */   }
/*     */ 
/*     */   public static Element set(Element pDestination, String pName, boolean pValue)
/*     */   {
/* 227 */     return set(pDestination, pName, String.valueOf(pValue));
/*     */   }
/*     */ 
/*     */   public static Element set(Element pDestination, String pName, int pValue) {
/* 231 */     return set(pDestination, pName, String.valueOf(pValue));
/*     */   }
/*     */ 
/*     */   public static Element set(Element pDestination, String pName, long pValue) {
/* 235 */     return set(pDestination, pName, String.valueOf(pValue));
/*     */   }
/*     */ 
/*     */   public static Element set(Element pDestination, String pName, float pValue) {
/* 239 */     return set(pDestination, pName, String.valueOf(pValue));
/*     */   }
/*     */ 
/*     */   public static Element set(Element pDestination, String pName, double pValue) {
/* 243 */     return set(pDestination, pName, String.valueOf(pValue));
/*     */   }
/*     */ 
/*     */   public static Element set(Element pDestination, String pName, Object pValue)
/*     */   {
/* 248 */     Element wNe = add(pDestination, pName);
/*     */ 
/* 250 */     if (pValue != null) {
/* 251 */       wNe.setTextContent(pValue.toString());
/*     */     }
/*     */ 
/* 254 */     return wNe;
/*     */   }
/*     */ 
/*     */   public static Element set(Element pDestination, String pName, KJXmlSerializable pSource)
/*     */   {
/* 260 */     if (pSource != null)
/*     */     {
/* 262 */       Element wTe = add(pDestination, pName);
/*     */ 
/* 264 */       pSource.writeXml(wTe);
/*     */ 
/* 266 */       return wTe;
/*     */     }
/*     */ 
/* 270 */     return null;
/*     */   }
/*     */ 
/*     */   public static void setObject(Element pDestination, String pName, KJXmlSerializable pSource)
/*     */   {
/* 276 */     Element wTe = set(pDestination, pName, pSource);
/*     */ 
/* 278 */     if (wTe != null)
/* 279 */       wTe.setAttribute("class", pSource.getClass().getName());
/*     */   }
/*     */ 
/*     */   public static Object toObject(Element pSource)
/*     */   {
/* 286 */     if (pSource != null)
/*     */     {
/* 288 */       KJXmlSerializable wTarget = createTargetFor(pSource);
/*     */ 
/* 290 */       wTarget.readXml(pSource);
/*     */ 
/* 292 */       return wTarget;
/*     */     }
/*     */ 
/* 296 */     return null;
/*     */   }
/*     */ 
/*     */   public static void saveDocument(Document pDocument, OutputStream pOs)
/*     */     throws IOException
/*     */   {
/* 302 */     pOs.write(xmlToString(pDocument).getBytes());
/*     */ 
/* 304 */     pOs.close();
/*     */   }
/*     */ 
/*     */   public static String xmlToString(Document pDocument)
/*     */     throws IOException
/*     */   {
/* 310 */     OutputFormat wOf = new OutputFormat("XML", "ISO-8859-1", true);
/*     */ 
/* 312 */     wOf.setIndent(4);
/*     */ 
/* 314 */     StringWriter wSw = new StringWriter();
/*     */ 
/* 316 */     XMLSerializer wXs = new XML11Serializer(wSw, wOf);
/*     */ 
/* 318 */     wXs.serialize(pDocument);
/*     */ 
/* 320 */     return wSw.toString();
/*     */   }
/*     */ 
/*     */   public static class ElementIterator implements Iterator {
/*     */     private TreeWalker treeWalker;
/*     */     private Node node;
/*     */ 
/*     */     public ElementIterator(TreeWalker pTw) {
/* 330 */       this.treeWalker = pTw;
/* 331 */       this.node = this.treeWalker.firstChild();
/*     */     }
/*     */ 
/*     */     public boolean hasNext() {
/* 335 */       return this.node != null;
/*     */     }
/*     */ 
/*     */     public Object next()
/*     */     {
/* 343 */       Node wNode = this.node;
/*     */ 
/* 345 */       this.node = this.treeWalker.nextSibling();
/*     */ 
/* 347 */       return wNode;
/*     */     }
/*     */ 
/*     */     public void remove()
/*     */     {
/* 355 */       throw new UnsupportedOperationException();
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.xml.KJXmlToolkit
 * JD-Core Version:    0.6.0
 */