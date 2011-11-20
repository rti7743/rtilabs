//hacked by rti
/*
 * Copyright (c) rti    ‚¨[‚é‚ç‚¢‚Æ‚è‚´[‚Ô
 * 
 * $Id$
 */
using System;
using System.Xml;
using System.IO;
using System.Drawing;
using System.Diagnostics;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Security.Cryptography;

using Travis.Storage;

using System.Data;
using System.Data.OleDb;

namespace Zanetti.CodeExtendData{

    //î•ñ
    abstract class DataInterface : IComparable
    {
        public abstract int getDate();
        public abstract void Read(XmlNode inX);
        public abstract XmlNode Write(XmlDocument ioDocument);

        public int CompareTo(object obj)
        {
            if (this.getDate() > ((DataInterface)obj).getDate())
            {
                return 1;
            }
            else if (this.getDate() < ((DataInterface)obj).getDate())
            {
                    return -1;
            }
            return 0;
        }

        public virtual int Compare(object x, object y)
        {
            if (((DataInterface)x).getDate() < ((DataInterface)y).getDate())
            {
                return -1;
            }
            else if (((DataInterface)x).getDate() > ((DataInterface)y).getDate())
            {
                return 1;
            }
            return 0;
        }
    }

    //ƒtƒ@ƒ“ƒ_ƒƒ“ƒ^ƒ‹‚Èî•ñ‚ğ‚Ü‚Æ‚ß‚Ä•Ô‚µ‚Ü‚·B
    class DataBasket
    {
        public int Kettusan = 0;       //ŒˆZ“ú yyyymmdd
        public int Uriage = 0;         //”„ã(•S–œ‰~)
        public int Keijyourieki = 0;   //Œoí—˜‰v(•S–œ‰~)
        public int Rieki = 0;          //—˜‰v(•S–œ‰~)
        public int Hitoikabueki = 0;   //ˆêŠ”‰v(‘K)
        public int Haitoukin = 0;      //”z“–‹à(‘K)
        public int Hitokabujyunsisan = 0;   //ˆêŠ”ƒ‘Y
        public int ZenkiKettusan = 0;   //‘OŠúŒˆZ“ú yyyymmdd
    };

    //ŒˆZî•ñ
    class DataAccounts : DataInterface
    {
        public int Code;
        public int Date;
        public int Kubun;               //ŒˆZ‹æ•ª 0:1/4l”¼Šú    1:2/4’†ŠÔŒˆZ   2:3/4l”¼Šú     3:4/4–{ŒˆZ
        public int ZenkiKettusan;       //‘OŠúŒˆZ“ú
//        public int ZenkiUriage;         //‘OŠú”„ã(•S–œ‰~)
//        public int ZenkiKeijyourieki;   //‘OŠúŒoí—˜‰v(•S–œ‰~)
//        public int Zenkirieki;          //‘OŠú—˜‰v(•S–œ‰~)
//        public int ZenkiHitoikabueki;   //‘OŠúˆêŠ”‰v(‘K)
//        public int ZenkiHaitoukin;      //‘OŠú”z“–‹à(‘K)
        public int KonkiKettusan;       //¡ŠúŒˆZ
        public int KonkiUriage;         //¡Šú”„ã(•S–œ‰~)
        public int KonkiKeijyourieki;   //¡ŠúŒoí—˜‰v(•S–œ‰~)
        public int Konkirieki;          //¡Šú—˜‰v(•S–œ‰~)
        public int KonkiHitoikabueki;   //¡ŠúˆêŠ”‰v(‘K)
        public int KonkiHaitoukin;      //¡Šú”z“–‹à(‘K)
        public int JikiKettusan;        //ŸŠúŒˆZ
        public int JikiUriage;          //ŸŠú”„ã(•S–œ‰~)
        public int JikiKeijyourieki;    //ŸŠúŒoí—˜‰v(•S–œ‰~)
        public int Jikirieki;           //ŸŠú—˜‰v(•S–œ‰~)
        public int JikiHitoikabueki;    //ŸŠúˆêŠ”‰v(‘K)
        public int JikiHaitoukin;       //ŸŠú”z“–‹à(‘K)
        public int Hitokabujyunsisan;   //ˆêŠ”ƒ‘Y



        public override int getDate()
        {
            return this.Date;
        }

        //“Ç‚İ‚İ
        public override void Read(XmlNode inX)
        {
            if (inX.Name != "ŒˆZ") return;

            this.Date = int.Parse(inX.Attributes["“ú•t"].InnerText);
            this.Code = int.Parse(inX.Attributes["ƒR[ƒh"].InnerText);
            this.Kubun = int.Parse(inX["‹æ•ª"].InnerText);
            this.ZenkiKettusan = int.Parse(inX["‘OŠúŒˆZ“ú"].InnerText);
//            this.ZenkiUriage = int.Parse(inX["‘OŠú”„ã"].InnerText);
//            this.ZenkiKeijyourieki = int.Parse(inX["‘OŠúŒoí—˜‰v"].InnerText);
//            this.Zenkirieki = int.Parse(inX["‘OŠú—˜‰v"].InnerText);
//            this.ZenkiHitoikabueki = int.Parse(inX["‘OŠúˆêŠ”‰v"].InnerText);
//            this.ZenkiHaitoukin = int.Parse(inX["‘OŠú”z“–‹à"].InnerText);
            this.KonkiKettusan = int.Parse(inX["¡ŠúŒˆZ“ú"].InnerText);
            this.KonkiUriage = int.Parse(inX["¡Šú”„ã"].InnerText);
            this.KonkiKeijyourieki = int.Parse(inX["¡ŠúŒoí—˜‰v"].InnerText);
            this.Konkirieki = int.Parse(inX["¡Šú—˜‰v"].InnerText);
            this.KonkiHitoikabueki = int.Parse(inX["¡ŠúˆêŠ”‰v"].InnerText);
            this.KonkiHaitoukin = int.Parse(inX["¡Šú”z“–‹à"].InnerText);
            this.JikiKettusan = int.Parse(inX["ŸŠúŒˆZ“ú"].InnerText);
            this.JikiUriage = int.Parse(inX["ŸŠú”„ã"].InnerText);
            this.JikiKeijyourieki = int.Parse(inX["ŸŠúŒoí—˜‰v"].InnerText);
            this.Jikirieki = int.Parse(inX["ŸŠú—˜‰v"].InnerText);
            this.JikiHitoikabueki = int.Parse(inX["ŸŠúˆêŠ”‰v"].InnerText);
            this.JikiHaitoukin = int.Parse(inX["ŸŠú”z“–‹à"].InnerText);
            this.Hitokabujyunsisan = int.Parse(inX["ˆêŠ”ƒ‘Y"].InnerText);
        }


        public override XmlNode Write(XmlDocument ioDocument)
        {
            XmlNode element = ioDocument.CreateElement("ŒˆZ");
            element.Attributes.Append(UtilX.CreateAttribute(ioDocument, "“ú•t", this.Date.ToString()));
            element.Attributes.Append(UtilX.CreateAttribute(ioDocument, "ƒR[ƒh", this.Code.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "‹æ•ª", this.Kubun.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "‘OŠúŒˆZ“ú", this.ZenkiKettusan.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "‘OŠú”„ã", this.ZenkiUriage.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "‘OŠúŒoí—˜‰v", this.ZenkiKeijyourieki.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "‘OŠú—˜‰v", this.Zenkirieki.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "‘OŠúˆêŠ”‰v", this.ZenkiHitoikabueki.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "‘OŠú”z“–‹à", this.ZenkiHaitoukin.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "¡ŠúŒˆZ“ú", this.KonkiKettusan.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "¡Šú”„ã", this.KonkiUriage.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "¡ŠúŒoí—˜‰v", this.KonkiKeijyourieki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "¡Šú—˜‰v", this.Konkirieki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "¡ŠúˆêŠ”‰v", this.KonkiHitoikabueki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "¡Šú”z“–‹à", this.KonkiHaitoukin.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "ŸŠúŒˆZ“ú", this.JikiKettusan.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "ŸŠú”„ã", this.JikiUriage.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "ŸŠúŒoí—˜‰v", this.JikiKeijyourieki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "ŸŠú—˜‰v", this.Jikirieki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "ŸŠúˆêŠ”‰v", this.JikiHitoikabueki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "ŸŠú”z“–‹à", this.JikiHaitoukin.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "ˆêŠ”ƒ‘Y", this.Hitokabujyunsisan.ToString()));

            return element;
        }
    };


     //‹ÆÑC³
     class DataRevised : DataInterface
     {
         public int Code;
         public int Date;
         public int SyuseimaeUriage;    //C³‘O”„ã
         public int Uriage;             //”„ã
         public int SyuseimaeKeijyourieki;    //C³‘OŒoí—˜‰v
         public int Keijyourieki;             //Œoí—˜‰v
         public int SyuseimaeZeibikieki;    //C³‘OÅˆø‰v
         public int Zeibikieki;             //Åˆø‰v
         public int SyuseimaeHaitou;    //C³‘O”z“–
         public int Haitou;             //”z“–

         public override int getDate()
         {
             return this.Date;
         }

         //“Ç‚İ‚İ
         public override void Read(XmlNode inX)
         {
             if (inX.Name != "‹ÆÑC³") return;

             this.Date = int.Parse(inX.Attributes["“ú•t"].InnerText);
             this.Code = int.Parse(inX.Attributes["ƒR[ƒh"].InnerText);
             this.SyuseimaeUriage = int.Parse(inX["C³‘O”„ã"].InnerText);
             this.Uriage = int.Parse(inX["”„ã"].InnerText);
             this.SyuseimaeKeijyourieki = int.Parse(inX["C³‘OŒoí—˜‰v"].InnerText);
             this.Keijyourieki = int.Parse(inX["Œoí—˜‰v"].InnerText);
             this.SyuseimaeZeibikieki = int.Parse(inX["C³‘OÅˆø‰v"].InnerText);
             this.Zeibikieki = int.Parse(inX["Åˆø‰v"].InnerText);
             this.SyuseimaeHaitou = int.Parse(inX["C³‘O”z“–"].InnerText);
             this.Haitou = int.Parse(inX["”z“–"].InnerText);
         }

         public override XmlNode Write(XmlDocument ioDocument)
         {
             XmlNode element = ioDocument.CreateElement("‹ÆÑC³");
             element.Attributes.Append(UtilX.CreateAttribute(ioDocument, "“ú•t", this.Date.ToString()));
             element.Attributes.Append(UtilX.CreateAttribute(ioDocument, "ƒR[ƒh", this.Code.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "C³‘O”„ã", this.SyuseimaeUriage.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "”„ã", this.Uriage.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "C³‘OŒoí—˜‰v", this.SyuseimaeKeijyourieki.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "Œoí—˜‰v", this.Keijyourieki.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "C³‘OÅˆø‰v", this.SyuseimaeZeibikieki.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "Åˆø‰v", this.Zeibikieki.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "C³‘O”z“–", this.SyuseimaeHaitou.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "”z“–", this.Haitou.ToString()));

             return element;
         }
     };


    //ƒR[ƒhŠg’£‚ğ‘©‚Ë‚éƒ{ƒX
    //q•ª‚Æ‚¨˜b‚µ‚½‚¢‚Æ‚«‚ÍAƒ{ƒX‚É˜b‚ğ’Ê‚·‚Æ‚¢‚¤‚Ì‚ªƒXƒW‚Æ‚¢‚¤‚à‚Ì
    //ƒRƒCƒc‚ÍƒR[ƒh(–Á•¿)’PˆÊ‚Å‘¶İ‚·‚é.
    class Data
    {
        private ArrayList AccountsHash; //ŒˆZî•ñ
        private ArrayList RevisedHash;  //‹ÆÑC³

        public Data()
        {
            this.AccountsHash = new ArrayList();
            this.RevisedHash = new ArrayList();
        }

        //“Ç‚İ‚İ
        public void Read(int inCode)
        {
            string filename = Util.GetExtDataFileName(inCode);

            if (!File.Exists(filename))
            {
                return;
            }
            XmlDocument document = new XmlDocument();
            document.Load(filename);
            XmlNodeList nodeList = document.DocumentElement.ChildNodes;
            foreach (XmlNode nd in nodeList)
            {
                if (nd.Name == "ŒˆZ")
                {
                    DataAccounts p = new DataAccounts();
                    p.Read(nd);
                    this.AccountsHash.Add(p);
                }
                else if (nd.Name == "‹ÆÑC³")
                {
                    DataRevised p = new DataRevised();
                    p.Read(nd);
                    this.RevisedHash.Add(p);
                }
            }

        }
        //‘‚«‚İ
        public void Write(int inCode)
        {
            bool change = false;

            string filename = Util.GetExtDataFileName(inCode);
            XmlDocument document = new XmlDocument();
            document.AppendChild(document.CreateXmlDeclaration("1.0", "UTF-8", null));
            document.AppendChild(document.CreateElement("ƒR[ƒhŠg’£"));

            foreach (DataAccounts p in this.AccountsHash)
            {
                XmlNode element = p.Write(document);
                document.DocumentElement.AppendChild(element);
                change = true;
            }
            foreach (DataRevised p in this.RevisedHash)
            {
                XmlNode element = p.Write(document);
                document.DocumentElement.AppendChild(element);
                change = true;
            }

            if (change)
            {
                document.Save(filename);
            }
        }

        //ŒˆZî•ñ‚ÌC³
        public void Update( DataAccounts inAccounts)
        {
            UpdateLaw(inAccounts, this.AccountsHash);
        }
        //‹ÆÑC³
        public void Update(DataRevised inRevised)
        {
            UpdateLaw(inRevised, this.RevisedHash);
        }

        //C³‚ğs‚¤ÀÛ‚Ì•”•ª
        private void UpdateLaw(DataInterface inData, ArrayList inArrayList)
        {
            foreach (DataInterface p in inArrayList)
            {
                if (p.getDate() == inData.getDate() )
                {
                    inArrayList.Remove(p);
                    inArrayList.Add(inData);
                    inArrayList.Sort();
                    return;
                }
            }
            //‚È‚¢ê‡‚Í’Ç‰Á‚·‚é.
            inArrayList.Add(inData);
            inArrayList.Sort();
        }

        //‹ß‚¢ƒf[ƒ^‚ğŒ©‚Â‚¯o‚·
        public DataAccounts FindAccounts(int inDate )
        {
            return (DataAccounts)FindLaw(inDate, this.AccountsHash);
        }
        //‹ß‚¢ƒf[ƒ^‚ğŒ©‚Â‚¯o‚·
        public DataRevised FindRevised(int inDate )
        {
            return (DataRevised)FindLaw(inDate,this.RevisedHash);
        }

        //ˆê”Ô‡—“I‚¾‚Æv‚í‚ê‚éŒˆZA‹ÆÑC³‚ğŒ©‚Â‚¯o‚·
        private DataBasket FindFundamentalSub(int inDate)
        {
            //‹ßê‚ÌŒˆZ‚Æ‹ßê‚Ì‹ÆÑC³‚ğ‹‚ß‚é
            DataAccounts accounts = FindAccounts(inDate);
            DataRevised revised = FindRevised(inDate);

            DataBasket  ret = new DataBasket();

            if (accounts == null && revised == null)
            {
                return ret; //î•ñ‚ª‚È‚¢!!
            }
            if (accounts == null)
            {//‹ÆÑC³‚ğ•Ô‚·
                ret.Kettusan = revised.Date;
                ret.Uriage = revised.Uriage;
                ret.Keijyourieki = revised.Keijyourieki;
                ret.Rieki = revised.Zeibikieki;
                ret.Hitoikabueki = 0;
                ret.Haitoukin = revised.Haitou;
                ret.Hitokabujyunsisan = 0;
                ret.ZenkiKettusan = 0;
                return ret;
            }
            if (revised == null)
            {//ŒˆZî•ñ‚ğ•Ô‚·
                ret.Kettusan = accounts.JikiKettusan;
                ret.Uriage = accounts.JikiUriage;
                ret.Keijyourieki = accounts.JikiKeijyourieki;
                ret.Rieki = accounts.Jikirieki;
                ret.Hitoikabueki = accounts.JikiHitoikabueki;
                ret.Haitoukin = accounts.JikiHaitoukin;
                ret.Hitokabujyunsisan = accounts.Hitokabujyunsisan;
                ret.ZenkiKettusan = accounts.ZenkiKettusan;
                return ret;
            }

            if (accounts.getDate() >= revised.getDate() )
            {
                if (accounts.JikiUriage == 0 && accounts.JikiKeijyourieki == 0 && accounts.Jikirieki == 0)
                {
                    //ŠúŒˆZî•ñ‚ª”’†!!!
                    //d•û‚È‚¢‚Ì‚ÅA‹ÆÑC³‚ğ•Ô‚·
                }
                else
                {
                    //ŒˆZî•ñ‚ğ•Ô‚·
                    ret.Kettusan = accounts.JikiKettusan;
                    ret.Uriage = accounts.JikiUriage;
                    ret.Keijyourieki = accounts.JikiKeijyourieki;
                    ret.Rieki = accounts.Jikirieki;
                    ret.Hitoikabueki = accounts.JikiHitoikabueki;
                    ret.Haitoukin = accounts.JikiHaitoukin;
                    ret.Hitokabujyunsisan = accounts.Hitokabujyunsisan;
                    ret.ZenkiKettusan = accounts.ZenkiKettusan;

                    return ret;
                }
            }
            //‹ÆÑC³‚ğ•Ô‚·
            ret.Kettusan = revised.Date;
            ret.Uriage = revised.Uriage;
            ret.Keijyourieki = revised.Keijyourieki;
            ret.Rieki = revised.Zeibikieki;
            ret.Hitoikabueki = (accounts.JikiHitoikabueki * revised.Zeibikieki) / accounts.Konkirieki;
            ret.Haitoukin = revised.Haitou;
            //ˆêŠ”‘Y‚ª‹ÆÑC³‚É‚È‚¢‚Ì‚ÅAŒˆZî•ñ‚©‚ç‹tZ‚µ‚Ä‹‚ß‚Ü‚·B
            //“r’†‚Å‘‘‚ğ‚â‚ç‚ê‚½ê‡A”š‚ª‚¨‚©‚µ‚­‚È‚è‚Ü‚·B
            ret.Hitokabujyunsisan = (accounts.Hitokabujyunsisan * revised.Zeibikieki) / accounts.Konkirieki;
            ret.ZenkiKettusan = accounts.ZenkiKettusan;

            return ret;
        }
        //w’è”N”‘O‚Ìˆê”Ô‡—“I‚¾‚Æv‚í‚ê‚éŒˆZA‹ÆÑC³‚ğŒ©‚Â‚¯o‚·
        public DataBasket FindFundamental(int inDate,int inSkip )
        {
            DataBasket NullBasket = new DataBasket();
            DataBasket basket = FindFundamentalSub(inDate);

            for (int i = 0; i < inSkip; i++)
            {
                if (basket.ZenkiKettusan == 0)
                {
                    return NullBasket;  //÷•à‚ª‚È‚¢‚Ì‚Å‹ó‚ğ•Ô‚µ‚Ü‚·
                }
                basket = FindFundamentalSub( basket.ZenkiKettusan );
            }
            return basket;
        }

        //‚·‚Å‚ÉŠm’è‚µ‚Ä‚¢‚éˆê”Ô‡—“I‚¾‚Æv‚í‚ê‚éŒˆZ‚ğŒ©‚Â‚¯o‚·
        private DataBasket FindFixedFundamentalSub(int inDate)
        {
            //‹ßê‚ÌŒˆZ‚Æ‹ßê‚Ì‹ÆÑC³‚ğ‹‚ß‚é
            DataAccounts accounts = FindAccounts(inDate);

            DataBasket ret = new DataBasket();

            if (accounts == null)
            {
                return ret; //î•ñ‚ª‚È‚¢!!
            }
            //ŒˆZî•ñ‚ğ•Ô‚·
            ret.Kettusan = accounts.KonkiKettusan;
            ret.Uriage = accounts.KonkiUriage;
            ret.Keijyourieki = accounts.KonkiKeijyourieki;
            ret.Rieki = accounts.Konkirieki;
            ret.Hitoikabueki = accounts.KonkiHitoikabueki;
            ret.Haitoukin = accounts.KonkiHaitoukin;
            ret.Hitokabujyunsisan = accounts.Hitokabujyunsisan;
            ret.ZenkiKettusan = accounts.ZenkiKettusan;

            return ret;
        }

        //‚·‚Å‚ÉŠm’è‚µ‚Ä‚¢‚éw’è”N”‘O‚Ìˆê”Ô‡—“I‚¾‚Æv‚í‚ê‚éŒˆZ‚ğŒ©‚Â‚¯o‚·
        public DataBasket FindFixedFundamental(int inDate, int inSkip)
        {
            DataBasket NullBasket = new DataBasket();
            DataBasket basket = FindFixedFundamentalSub(inDate);

            for (int i = 0; i < inSkip; i++)
            {
                if (basket.ZenkiKettusan == 0)
                {
                    return NullBasket;  //÷•à‚ª‚È‚¢‚Ì‚Å‹ó‚ğ•Ô‚µ‚Ü‚·
                }
                basket = FindFixedFundamentalSub(basket.ZenkiKettusan);
            }
            return basket;
        }

        //‹ß‚¢ƒf[ƒ^‚ğŒ©‚Â‚¯o‚·
        private DataInterface FindLaw(int inDate, ArrayList inArrayList)
        {
            DataInterface last = null;
            //”z—ñ‚Í¸‡‚Éƒ\[ƒg‚³‚ê‚Ä‚¢‚é‚½‚ßA©•ª‚æ‚è‘å‚«‚¢‚à‚Ì‚ªŒ©‚Â‚©‚é‚Ü‚Å”äŠr‚·‚é.
            foreach (DataInterface p in inArrayList)
            {
                if (inDate <= p.getDate() )
                {
                    return last;
                }
                last = p;
            }
            //‹ß‚¢ƒf[ƒ^‚Í‚È‚¢
            return last;
        }
    };

    //‹¤’Ê•”•ª‚©‚ÈA‚©‚È
    public class UtilX
    {
        public static XmlElement CreateElemnt(XmlDocument ioDocument, string inName, string inValue)
        {
            XmlElement e = ioDocument.CreateElement(inName);
            XmlText text = ioDocument.CreateTextNode(inValue);
            e.AppendChild(text);
            return e;
        }
        public static XmlAttribute CreateAttribute(XmlDocument ioDocument, string inName, string inValue)
        {
            XmlAttribute e = ioDocument.CreateAttribute(inName);
            e.Value = inValue;
            return e;
        }
        
        /*
         * //ADO ”Å
        //Hashtable[ArrayList(ArrayList)]‚Á‚Ä\‘¢‚Å•Ô‚Á‚Ä‚­‚é(w
        //Hashtable[•\‚Ì–¼‘O][s][—ñ] ‚Æ‚¢‚¤\¬‚©‚ÈAAA
        //‹Éˆ«‚¾‚ÈB
        public static Hashtable XLSReader(string inFilename)
        {
            //ADO ƒIƒuƒWƒFƒNƒg‚­‚ñ‚ğì¬‚µ‚Ü‚·B
            ADODB.Connection connect = new ADODB.Connection();
            ADOX.Catalog catalog = new ADOX.Catalog();
            //ƒGƒNƒZƒ‹ƒtƒ@ƒCƒ‹‚ÉÚ‘±‚µ‚Ü‚·B
            connect.Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + inFilename + ";" + " Extended Properties=Excel 8.0;","","",0);

            catalog.ActiveConnection = connect;
            Hashtable returnValue = new Hashtable();

            string tablename ; 
            for(int t = 0 ; t < catalog.Tables.Count ; t ++)
            {
                //ƒe[ƒuƒ‹(•\)
                if ( catalog.Tables[t].Type != "TABLE" )
                {
                    continue;   //ƒe[ƒuƒ‹‚À‚á‚È‚¢‚æB
                }
                //ƒe[ƒuƒ‹‚ªŒ©‚Â‚©‚Á‚½‚Ì‚ÅA‚»‚¢‚Â‚ÉƒNƒGƒŠ‚ğ•ú‚¿‚Ü‚·B
                tablename = catalog.Tables[t].Name;

			    //SQL‚Ì‚æ‚¤‚È‚à‚Ì ‚ğ\’z.
			    string sql = "SELECT * FROM `" + tablename + "`";

			    //SQL‚Ì@‚·‚²‚¢ Às
                object temp;    //‰½‚©•Ô‚µ‚½‚¢‚ç‚µ‚¢A“K“–‚É“ü‚ê‚é
                ADODB.Recordset recordset = connect.Execute(sql, out temp, (int)ADODB.ExecuteOptionEnum.adOptionUnspecified);
    			
                //s
                ArrayList arrayY = new ArrayList();
                for(int line = 1 ;  ! recordset.EOF ; line ++ )
                {
                    //—ñ
                    ArrayList arrayX = new ArrayList();
                    for (int fields = 0; fields < recordset.Fields.Count; fields++)
                    {
                        arrayX.Add( recordset.Fields[fields].Value.ToString()   );
                    }
                    
                    arrayY.Add(arrayX);

                    //Ÿ‚Ìs‚Ö
                    recordset.MoveNext();
                }

                returnValue[tablename] = arrayY;
                //ŠJ•ú
                recordset.Close();
                recordset = null;
                temp = null;
            }
            connect.Close();
            connect = null;

            catalog = null;


            return returnValue;
        }
        */
        //ADO.NET”Å
        //Hashtable[ArrayList(ArrayList)]‚Á‚Ä\‘¢‚Å•Ô‚Á‚Ä‚­‚é(w
        //Hashtable[•\‚Ì–¼‘O][s][—ñ] ‚Æ‚¢‚¤\¬‚©‚ÈAAA
        //‹Éˆ«‚¾‚ÈB
        public static Hashtable XLSReader(string inFilename)
        {
            //‚±‚ê‚ÉŒ‹‰Ê‚ğ‘ã“ü‚µ‚Ü‚·.
            Hashtable returnValue = new Hashtable();

            OleDbConnection connect = new OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + inFilename + ";" + " Extended Properties=Excel 8.0;");
            //Ú‘±ŠJn
            connect.Open();

            //ƒe[ƒuƒ‹–¼‚ğ‹‚ß‚Ü‚·
            string tablename;
            DataTable schemaTable = connect.GetOleDbSchemaTable(OleDbSchemaGuid.Tables,
                          new Object[] { null, null, null, "TABLE" });
            foreach (DataRow schemaTableRow in schemaTable.Rows)
            {
                //ƒe[ƒuƒ‹‚Ì–¼‘O
                tablename = schemaTableRow["TABLE_NAME"].ToString();

                //SQL‚Ì‚æ‚¤‚È‚à‚Ì ‚ğ\’z‚µ‚Ü‚·.
                string sql = "SELECT * FROM `" + tablename + "`";
                OleDbCommand command = connect.CreateCommand();
                command.CommandText = sql;

                OleDbDataAdapter adapter = new OleDbDataAdapter(command);
                DataSet dts = new DataSet();
                adapter.Fill(dts);

                DataTable table = dts.Tables[0];

                //s
                ArrayList arrayY = new ArrayList();

                //ADO‚Æ‚ÌŒİŠ·«‚ğæ‚é‚½‚ß‚Éƒwƒbƒ_[‚àŒ‹‰Ê‚ÉŠÜ‚ß‚Ü‚·B
                {
                    ArrayList arrayX = new ArrayList();
                    foreach (DataColumn column in table.Columns)
                    {
                        arrayX.Add(column.ToString());
                    }
                    arrayY.Add(arrayX);
                }

                foreach (DataRow row in table.Rows)
                {
                    //—ñ
                    ArrayList arrayX = new ArrayList();
                    foreach (DataColumn column in table.Columns)
                    {
                        arrayX.Add(row[column].ToString());
                    }
                    arrayY.Add(arrayX);
                }
                returnValue[tablename] = arrayY;
            }

            connect.Close();
            connect = null; //ŠJ•ú`

            return returnValue;
        }
    };
}

//------------------