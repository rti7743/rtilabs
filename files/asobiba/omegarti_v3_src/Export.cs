/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;

using Zanetti.Commands;

namespace Zanetti.Data
{
    internal enum ExportFormat {
        CSV,
        XML
    }

   
    /// <summary>
    /// Export ÇÃäTóvÇÃê‡ñæÇ≈Ç∑ÅB
	/// </summary>
	internal abstract class DataExporter
	{
		protected TextWriter _writer;

		public abstract void StartDocument();
		public abstract void EndDocument();
		public abstract void StartRecord(TradeData data);
		public abstract void EndRecord(TradeData data);
		public abstract void WriteDelimiter();
		public abstract void WriteElement(string name, string value);

		public DataExporter(TextWriter w) {
			_writer = w;
		}
		public void Close() {
			_writer.Close();
		}
		public static void ExportData(string filename, DataFarm farm, ExportFormat format) {
			TextWriter wr = null;
			try {
				wr = new StreamWriter(filename, false, System.Text.Encoding.Default);
				DataExporter e;
				if(format==ExportFormat.CSV)
					e = new CSVDataExporter(wr);
				else
					e = new XMLDataExporter(wr);

				e.StartDocument();
				for(int i=0; i<farm.FilledLength; i++)
                {
                    try
                    {
                        e.WriteRecord(farm.GetByIndex(i));
                    }
                    catch(Exception)
                    {
                    }
                }       
				e.EndDocument();
			}
                
			catch(Exception ex) {
				Util.ReportCriticalError(ex);
				Util.Warning(Env.Frame, ex.Message);
			}
			finally {
				if(wr!=null) wr.Close();
			}
		}


		private void WriteRecord(TradeData data) {
			StartRecord(data);
			WriteElement("date",   Util.FormatShortDate(data.Date));
			WriteDelimiter();
			WriteElement("open",   data.Open.ToString());
			WriteDelimiter();
			WriteElement("high",   data.High.ToString());
			WriteDelimiter();
			WriteElement("low",    data.Low.ToString());
			WriteDelimiter();
			WriteElement("close",  data.Close.ToString());
			WriteDelimiter();
			WriteElement("volume", data.Volume.ToString());
			EndRecord(data);
		}
	}

	internal class CSVDataExporter : DataExporter {
		public CSVDataExporter(TextWriter w) : base(w) {
		}
		public override void StartDocument() {
			_writer.WriteLine("date,open,high,low,close,volume");
		}
		public override void EndDocument() {
		}

		public override void StartRecord(TradeData data) {
		}
		public override void EndRecord(TradeData data) {
			_writer.WriteLine();
		}

		public override void WriteDelimiter() {
			_writer.Write(",");			
		}
		public override void WriteElement(string name, string value) {
			_writer.Write(value);
		}
	}
	internal class XMLDataExporter : DataExporter {
		public XMLDataExporter(TextWriter w) : base(w) {
		}
		public override void StartDocument() {
			_writer.WriteLine("<?xml version=\"1.0\" encoding=\""+_writer.Encoding.WebName + "\"?>");
			_writer.WriteLine("<omegachart-trade-data>");
		}
		public override void EndDocument() {
			_writer.WriteLine("</omegachart-trade-data>");
		}

		public override void StartRecord(TradeData data) {
			_writer.Write("  <record>");
		}
		public override void EndRecord(TradeData data) {
			_writer.WriteLine("</record>");
		}

		public override void WriteDelimiter() {
		}
		public override void WriteElement(string name, string value) {
			_writer.Write("<");
			_writer.Write(name);
			_writer.Write(">");
			_writer.Write(value);
			_writer.Write("</");
			_writer.Write(name);
			_writer.Write(">");
		}
	}
}
