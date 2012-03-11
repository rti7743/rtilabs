#include "XLWaveUtil.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <windows.h>

//音声操作
//このプログラムは、「C言語ではじめる音のプログラミング」をベースしています。
//いい本なのでamazon で買いましょう。
//http://www.amazon.co.jp/dp/4274206505
class XLWave
{
private:
	unsigned int fs;		// 標本化周波数
	unsigned int bits;	// 量子化精度 
	unsigned int length; // 音データの長さ
	double *l;  // 音データ
	double *r;  // 音データ(モノラルの場合、NULLになる.)
public:
	// 標本化周波数
	unsigned int getSamplesPerSec() const
	{
		return this->fs;
	}
	// 量子化精度
	unsigned int getBitsPerSample() const
	{
		return this->bits;
	}
	// 音データの長さ
	unsigned int getLength() const
	{
		return this->length;
	}
	unsigned int getChannel() const
	{
		return IsStereo() ? 2 : 1;
	}
	// 音データ 左
	const double* getDataL() const
	{
		this->l;
	}
	// 音データ 右
	const double* getDataR() const
	{
		this->r;
	}
	// 音データ 左 constなし
	double* getLowDataL() 
	{
		this->l;
	}
	// 音データ  constなし
	double* getLowDataR() 
	{
		this->r;
	}
	bool IsStereo() const
	{
		return this->r != NULL;
	}
	bool IsEnable() const
	{
		return this->l != NULL;
	}

	XLWave()
	{
		this->fs = 0;
		this->bits = 0;
		this->length = 0;
		this->l = NULL;
		this->r = NULL;
	}
	XLWave(const XLWave& b)
	{
		this->fs = b.fs;
		this->bits = b.bits;
		this->length = b.length;
		if (b.l == NULL)
		{
			this->l = NULL;
		}
		else
		{
			this->l = (double*)calloc(b.length, sizeof(double));
			memcpy(this->l,b.l,b.length * sizeof(double));
		}
		if (b.r == NULL)
		{
			this->r = NULL;
		}
		else
		{
			this->r = (double*)calloc(b.length, sizeof(double));
			memcpy(this->r,b.r,b.length * sizeof(double));
		}
	}

	XLWave(const std::string& filename)
	{
		Open(filename);
	}
	XLWave(const char* wavedata ,unsigned  int len)
	{
		Open(wavedata,len);
	}
	XLWave(const char* wavedata ,unsigned  int len,unsigned int samplesPerSec,unsigned int bitsPerSample,unsigned  int channel)
	{
		Open(wavedata ,  len, samplesPerSec, bitsPerSample,  channel);
	}
	XLWave(unsigned int len,unsigned int samplesPerSec,unsigned int bitsPerSample,unsigned  int channel)
	{
		MakeNullWave( len, samplesPerSec, bitsPerSample,  channel);
	}

	virtual ~XLWave()
	{
		if (this->l)
		{
			free(this->l);
			this->l = NULL;
		}
		if (this->r)
		{
			free(this->r);
			this->r = NULL;
		}
	}

	bool XLWave::Open(const char* wavedata ,unsigned  int dataChunkSize,int samplesPerSec,int bitsPerSample, int channel)
	{
		unsigned data;
		const char* now = wavedata;
		this->fs = samplesPerSec;		// 標本化周波数
		this->bits = bitsPerSample;	// 量子化精度 
		if (channel == 1)
		{//モノラル
			this->length = dataChunkSize / 2; // 音データの長さ
			this->l = (double*) calloc(this->length, sizeof(double)); // メモリの確保
			this->r = NULL;
  
			for (unsigned int n = 0; n < this->length; n++)
			{
				memcpy(&data , now , 2 ); now += 2;	//音データの読み取り
				this->l[n] = (double)data / 32768.0; // 音データを-1以上1未満の範囲に正規化する
			}
		}
		else
		{//ステレオ
			this->length = dataChunkSize / 4; // 音データの長さ
			this->l = (double*) calloc(this->length, sizeof(double)); //
			this->r = (double*) calloc(this->length, sizeof(double)); //
  
			for (unsigned int n = 0; n < this->length; n++)
			{
				memcpy(&data , now , 2 ); now += 2;	//音データの読み取り
				this->l[n] = (double)data / 32768.0; // 音データを-1以上1未満の範囲に正規化する

				memcpy(&data , now , 2 ); now += 2;	//音データの読み取り
				this->r[n] = (double)data / 32768.0; // 音データを-1以上1未満の範囲に正規化する
			}
		}
		return false;
	}

	bool XLWave::Open(const char* wavedata ,unsigned  int len) 
	{
		char riff_chunk_ID[4];
		long riff_chunk_size;
		char riff_form_type[4];
		char fmt_chunk_ID[4];
		long fmt_chunk_size;
		short fmt_wave_format_type;
		short fmt_channel;
		long fmt_samples_per_sec;
		long fmt_bytes_per_sec;
		short fmt_block_size;
		short fmt_bits_per_sample;
		char data_chunk_ID[4];
		unsigned long data_chunk_size;
		short data;

		if (len < 44)
		{
			return false;
		}
		const char* now = wavedata;
		memcpy(&riff_chunk_ID , now , 4 ); now += 4;
		memcpy(&riff_chunk_size , now , 4 ); now += 4;
		memcpy(&riff_form_type , now , 4 ); now += 4;
		memcpy(&fmt_chunk_ID , now , 4 ); now += 4;
		memcpy(&fmt_chunk_size , now , 4 ); now += 4;
		memcpy(&fmt_wave_format_type , now , 2 ); now += 2;
		memcpy(&fmt_channel , now , 2 ); now += 2;
		memcpy(&fmt_samples_per_sec , now , 4 ); now += 4;
		memcpy(&fmt_bytes_per_sec , now , 4 ); now += 4;
		memcpy(&fmt_block_size , now , 2 ); now += 2;
		memcpy(&fmt_bits_per_sample , now , 2 ); now += 2;
		memcpy(&data_chunk_ID , now , 4 ); now += 4;
		memcpy(&data_chunk_size , now , 4 ); now += 4;
		if (memcmp(riff_chunk_ID, "RIFF",4) != 0 )
		{
			return false;
		}
		if (memcmp(riff_form_type, "WAVE",4) != 0 )
		{
			return false;
		}
		if (memcmp(fmt_chunk_ID, "fmt ",4) != 0 )
		{
			return false;
		}
		if ( len - 44 < data_chunk_size)
		{
			return false;
		}
		if (fmt_channel >= 3)
		{
			return false;
		}

		this->fs = fmt_samples_per_sec;		// 標本化周波数
		this->bits = fmt_bits_per_sample;	// 量子化精度 
		if (fmt_channel == 1)
		{//モノラル
			this->length = data_chunk_size / 2; // 音データの長さ
			this->l = (double*) calloc(this->length, sizeof(double)); // メモリの確保
			this->r = NULL;
  
			for (unsigned int n = 0; n < this->length; n++)
			{
				memcpy(&data , now , 2 ); now += 2;	//音データの読み取り
				this->l[n] = (double)data / 32768.0; // 音データを-1以上1未満の範囲に正規化する
			}
		}
		else
		{//ステレオ
			this->length = data_chunk_size / 4; // 音データの長さ
			this->l = (double*) calloc(this->length, sizeof(double)); //
			this->r = (double*) calloc(this->length, sizeof(double)); //
  
			for (unsigned int n = 0; n < this->length; n++)
			{
				memcpy(&data , now , 2 ); now += 2;	//音データの読み取り
				this->l[n] = (double)data / 32768.0; // 音データを-1以上1未満の範囲に正規化する

				memcpy(&data , now , 2 ); now += 2;	//音データの読み取り
				this->r[n] = (double)data / 32768.0; // 音データを-1以上1未満の範囲に正規化する
			}
		}
		return true;
	}
	bool XLWave::Open(const std::string& wavFilename) 
	{
		FILE * fp = fopen(wavFilename.c_str() , "rb");
		//存在しない場合は空
		if (fp == NULL) return false;

		//ケツに持っていって.
		fseek(fp , 0 ,SEEK_END);

		//これでサイズがわかる.
		unsigned long size = ftell(fp);

		//先頭に戻す.
		fseek(fp , 0 ,SEEK_SET);

		char* buffer = new char[size];
		fread(buffer , 1 , size , fp);
		bool r = Open(buffer,size);

		delete [] buffer;
		return r;
	}
	bool XLWave::MakeNullWave(unsigned int len,unsigned int samplesPerSec,unsigned int bitsPerSample,unsigned int channel)
	{
		assert(channel == 1 || channel == 2);

		this->fs = samplesPerSec;		// 標本化周波数
		this->bits = bitsPerSample;	// 量子化精度 
		if (channel == 1)
		{//モノラル
			this->length = len; // 音データの長さ
			this->l = (double*) calloc(this->length, sizeof(double)); // メモリの確保
			this->r = NULL;
		}
		else
		{//ステレオ
			this->length = len; // 音データの長さ
			this->l = (double*) calloc(this->length, sizeof(double)); // メモリの確保
			this->r = (double*) calloc(this->length, sizeof(double)); // メモリの確保
		}
		return true;
	}

	bool XLWave::Save(const std::string& filename) 
	{
		assert(this->l);

		char riff_chunk_ID[4];
		long riff_chunk_size;
		char riff_form_type[4];
		char fmt_chunk_ID[4];
		long fmt_chunk_size;
		short fmt_wave_format_type;
		short fmt_channel;
		long fmt_samples_per_sec;
		long fmt_bytes_per_sec;
		short fmt_block_size;
		short fmt_bits_per_sample;
		char data_chunk_ID[4];
		long data_chunk_size;
		short data;
  
		riff_chunk_ID[0] = 'R';
		riff_chunk_ID[1] = 'I';
		riff_chunk_ID[2] = 'F';
		riff_chunk_ID[3] = 'F';
		riff_chunk_size = 36 + this->length * 2 * this->getChannel();
		riff_form_type[0] = 'W';
		riff_form_type[1] = 'A';
		riff_form_type[2] = 'V';
		riff_form_type[3] = 'E';
  
		fmt_chunk_ID[0] = 'f';
		fmt_chunk_ID[1] = 'm';
		fmt_chunk_ID[2] = 't';
		fmt_chunk_ID[3] = ' ';
		fmt_chunk_size = 16;
		fmt_wave_format_type = 1;
		fmt_channel = this->getChannel(); //ステレオ2 モノラル1？
		fmt_samples_per_sec = this->fs; // 標本化周波数 
		fmt_bytes_per_sec = this->fs * this->bits / 8 * this->getChannel();
		fmt_block_size = this->bits / 8 * this->getChannel();
		fmt_bits_per_sample = this->bits; // 量子化精度 
  
		data_chunk_ID[0] = 'd';
		data_chunk_ID[1] = 'a';
		data_chunk_ID[2] = 't';
		data_chunk_ID[3] = 'a';
		data_chunk_size = this->length * 2 * this->getChannel();
  
		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
		{
			return false;
		}
  
		fwrite(riff_chunk_ID, 1, 4, fp);
		fwrite(&riff_chunk_size, 4, 1, fp);
		fwrite(riff_form_type, 1, 4, fp);
		fwrite(fmt_chunk_ID, 1, 4, fp);
		fwrite(&fmt_chunk_size, 4, 1, fp);
		fwrite(&fmt_wave_format_type, 2, 1, fp);
		fwrite(&fmt_channel, 2, 1, fp);
		fwrite(&fmt_samples_per_sec, 4, 1, fp);
		fwrite(&fmt_bytes_per_sec, 4, 1, fp);
		fwrite(&fmt_block_size, 2, 1, fp);
		fwrite(&fmt_bits_per_sample, 2, 1, fp);
		fwrite(data_chunk_ID, 1, 4, fp);
		fwrite(&data_chunk_size, 4, 1, fp);
  
		if (this->IsStereo() )
		{//ステレオ
			for (unsigned int n = 0; n < this->length; n++)
			{
				double s = (this->l[n] + 1.0) / 2.0 * 65536.0;
				if (s > 65535.0)
				{
					s = 65535.0; // クリッピング
				}
				else if (s < 0.0)
				{
					s = 0.0;    // クリッピング
				}
    			data = (short)(s + 0.5) - 32768; // 四捨五入とオフセットの調節
				fwrite(&data, 2, 1, fp);         // 音データの書き出し

				s = (this->r[n] + 1.0) / 2.0 * 65536.0;
				if (s > 65535.0)
				{
					s = 65535.0; // クリッピング
				}
				else if (s < 0.0)
				{
					s = 0.0;    // クリッピング
				}
    			data = (short)(s + 0.5) - 32768; // 四捨五入とオフセットの調節
				fwrite(&data, 2, 1, fp);         // 音データの書き出し
			}
		}
		else
		{//モノラル
			for (unsigned int n = 0; n < this->length; n++)
			{
				double s = (this->l[n] + 1.0) / 2.0 * 65536.0;

				if (s > 65535.0)
				{
					s = 65535.0; // クリッピング
				}
				else if (s < 0.0)
				{
					s = 0.0;    // クリッピング
				}
    
				data = ((short)(s + 0.5)) - 32768; // 四捨五入とオフセットの調節
				fwrite(&data, 2, 1, fp);         // 音データの書き出し
			}
		}
		fclose(fp);
		return true;
	}

	//音の一部をくりとる substr みたいな。
	XLWave SubWave(unsigned int startFream,unsigned int step = 0) const
	{
		if (startFream > this->length)
		{
			return XLWave();
		}
		if (step == 0 || startFream + step > this->length)
		{//最後まで
			step = this->length - startFream;
		}
		XLWave ret( step , this->fs,this->bits , this->getChannel() );
		if (this->getChannel() == 2)
		{//ステレオ
			for(unsigned int i = 0 ; i < step ; ++i)
			{
				ret.l[i + startFream] = this->l[i];
				ret.r[i + startFream] = this->r[i];
			}
		}
		else
		{//モノラル
			for(unsigned int i = 0 ; i < step ; ++i)
			{
				ret.l[i + startFream] = this->l[i];
			}
		}
		return std::move(ret);
	}
	//最後の部分に音を加える.

	//ディレイ
	XLWave DelayEffect(double attenuationRate = 0.5,double delayTime = 0.385,int repeat = 2) const
	{
		//チャンネルがわかれてしまうので、共通式を作って、チャンネルごとに適応する.
		auto func = [](double* ss,const double* s,int len,double attenuationRate,double delayTime ,int repeat){
			for (int n = 0; n < len; n++)
			{
				ss[n] = s[n]; // 現在の時刻の音データ
				for (int i = 1; i <= repeat; i++)
				{
					int m = (int)((double)n - ((double)i * delayTime));
					if (m >= 0)
					{
						ss[n] += pow(attenuationRate, (double)i) * s[m]; // 過去の音データをミックスする
					}
				}
			}
		};

		delayTime *= this->fs; //遅延時間 * 標本化周波数

		//結果を格納するクラスを作成する
		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,attenuationRate,delayTime , repeat);
			func(ret.r , this->r,this->length,attenuationRate,delayTime , repeat);
		}
		else
		{
			func(ret.l , this->l,this->length,attenuationRate,delayTime , repeat);
		}

		return ret;
	}

	//トレモロ
	XLWave TremoloEffect(double depth, double rate) const
	{
		//チャンネルがわかれてしまうので、共通式を作って、チャンネルごとに適応する.
		auto func = [](double* ss,const double* s,int len,double depth,double rate){
			for (int n = 0; n < len; n++)
			{
//				double a = 1.0 + depth * sin(2.0 * M_PI * rate * n / fs);
				double a = 1.0 + depth * sin( rate * n); //2.0 * M_PI *rate/fsを先にやってしまう.
				ss[n] = a * s[n];
			}
		};

		rate = 2.0 * M_PI * rate / this->fs;

		//結果を格納するクラスを作成する
		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,depth,rate);
			func(ret.r , this->r,this->length,depth,rate);
		}
		else
		{
			func(ret.l , this->l,this->length,depth,rate);
		}

		return ret;
	}
	//コーラス
	XLWave ChorusEffect(double delayTime,double depth, double rate) const
	{
		//チャンネルがわかれてしまうので、共通式を作って、チャンネルごとに適応する.
		auto func = [](double* ss,const double* s,int len,double delayTime,double depth, double rate){
			for (int n = 0; n < len; n++)
			{
				ss[n] = s[n]; // 現在の時刻の音データ

//				double tau = delayTime + depth * sin(2.0 * M_PI * rate * n / this->fs);
				double tau = delayTime + depth * sin(rate * n );  //2.0 * M_PI *rate/fsを先にやってしまう.
				double t = (double)n - tau;
				int m = (int)t;
				double delta = t - (double)m;
				if (m >= 0 && m + 1 < len)
				{
					ss[n] += delta * s[m + 1] + (1.0 - delta) * s[m]; 
				}
			}
		};

		delayTime *= this->fs; //遅延時間 * 標本化周波数
		rate = 2.0 * M_PI * rate / this->fs;

		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,delayTime, depth,  rate);
			func(ret.r , this->r,this->length,delayTime, depth,  rate);
		}
		else
		{
			func(ret.l , this->l,this->length,delayTime, depth,  rate);
		}
		return ret;
	}


	//スピード変更
	XLWave SpeedUpEffect( double rate = 1.5 ) const
	{
		//チャンネルがわかれてしまうので、共通式を作って、チャンネルごとに適応する.
		auto func = [](double* ss,const double* s,int len,double rate,double fs){
  
			int template_size = (int)(fs * 0.01); // 10ms
			int pmin = (int)(fs * 0.005); // 5ms
			int pmax = (int)(fs * 0.02);  // 20ms
  
			double* x = (double*)calloc(template_size, sizeof(double)); // メモリの確保
			double* y = (double*)calloc(template_size, sizeof(double)); // メモリの確保
			double* r = (double*)calloc((pmax + 1), sizeof(double));    // メモリの確保
  
			int offset0 = 0;
			int offset1 = 0;
			bool isup = rate >= 1;
  
			while (offset0 + pmax * 2 < len)
			{
				for (int n = 0; n < template_size; n++)
				{
					x[n] = s[offset0 + n]; // 本来の音データ
				}
    
				double max_of_r = 0.0;
				int p = pmin;
				for (int m = pmin; m <= pmax; m++)
				{
					for (int n = 0; n < template_size; n++)
					{
						y[n] = s[offset0 + m + n]; // mサンプルずらした音データ
					}
					r[m] = 0.0;
					for (int n = 0; n < template_size; n++)
					{
						r[m] += x[n] * y[n]; // 相関関数
					}
					if (r[m] > max_of_r)
					{
						max_of_r = r[m]; // 相関関数のピーク
						p = m;			 // 音データの基本周期
					}
				}
    
				if (isup)
				{//スピードアップ
					for (int n = 0; n < p; n++)
					{
						ss[offset1 + n] = s[offset0 + n] * (p - n) / p; // 単調減少の重みづけ
						ss[offset1 + n] += s[offset0 + p + n] * n / p;  // 単調増加の重みづけ
					}
    
					int q = (int)(p / (rate - 1.0) + 0.5);
					for (int n = p; n < q; n++)
					{
						if (offset0 + p + n >= len)
						{
							break;
						}
						ss[offset1 + n] = s[offset0 + p + n];
					}
    
					offset0 += p + q; /* offset0の更新 */
					offset1 += q; /* offset1の更新 */
				}
				else
				{//スピードダウン
					for (int n = 0; n < p; n++)
					{
						ss[offset1 + n] = s[offset0 + n];
					}
					for (int n = 0; n < p; n++)
					{
						ss[offset1 + p + n] = s[offset0 + p + n] * (p - n) / p; /* 単調減少の重みづけ */
						ss[offset1 + p + n] += s[offset0 + n] * n / p; /* 単調増加の重みづけ */
					}
    
					int q = (int)(p * rate / (1.0 - rate) + 0.5);
					for (int n = p; n < q; n++)
					{
						if (offset0 + n >= len)
						{
						break;
						}
						ss[offset1 + p + n] = s[offset0 + n];
					}
    
					offset0 += q; /* offset0の更新 */
					offset1 += p + q; /* offset1の更新 */
				}
			}
  
			free(x); /* メモリの解放 */
			free(y); /* メモリの解放 */
			free(r); /* メモリの解放 */
  		};

		XLWave ret( this->length / rate + 1, this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,rate,this->fs);
			func(ret.r , this->r,this->length,rate,this->fs);
		}
		else
		{
			func(ret.l , this->l,this->length,rate,this->fs);
		}
		return ret;
	}
	//音量変更
	XLWave VolumeUpEffect( double rate = 1.5 ) const
	{
		//チャンネルがわかれてしまうので、共通式を作って、チャンネルごとに適応する.
		auto func = [](double* ss,const double* s,int len,double rate){
			for (int n = 0; n < len; n++)
			{
				ss[n] = s[n] * rate;
			}
		};

		//結果を格納するクラスを作成する
		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,rate);
			func(ret.r , this->r,this->length,rate);
		}
		else
		{
			func(ret.l , this->l,this->length,rate);
		}

		return ret;
	}
	//ピッチシフタ
	XLWave PitchShiftEffect( double pitch = 1.5) const
	{
		//チャンネルがわかれてしまうので、共通式を作って、チャンネルごとに適応する.
		auto func = [](double* ss,const double* s,int shortlen,int orignallen,double pitch){
			int J = 24;

			for (int n = 0; n < orignallen; n++)
			{
				double t = pitch * n;
				int offset = (int)t;
				for (int m = offset - J / 2; m <= offset + J / 2; m++)
				{
					if (m >= 0 && m < shortlen)
					{
						ss[n] += s[m] * sinc(M_PI * (t - m));
					}
				}
			}
		};

		//結果を格納するクラスを作成する
		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );

		//まず速度を変更します。
//		double rate = 0.66 * pitch / 1.5; //rateとpicthには少しさを付ける？ rate:pitch = 0.66:1.5
		double rate = 2.16 - pitch; //picth 0.66   rate 1.5
		                            //picth 1.5    rate 0.66
		XLWave tempWave = this->SpeedUpEffect(rate);
		
		//これで音の高さは変わりましたが、
		//音が早送/スロー再生したので音の長さが変わってしまったので、
		//引き伸ばし、収縮させます。
		if (this->IsStereo())
		{
			func(ret.l , tempWave.l,tempWave.length,ret.length,pitch);
			func(ret.r , tempWave.r,tempWave.length,ret.length,pitch);
		}
		else
		{
			func(ret.l , tempWave.l,tempWave.length,ret.length,pitch);
		}

		return ret;
	}
private:
	static double XLWave::sinc(double x)
	{
		double y;
  
		if (x == 0.0)
		{
			y = 1.0;
		}
		else
		{
			y = sin(x) / x;
		}
  
		return y;
	}
};

int main()
{
	XLWave a("sample12.wav");
	XLWave b = a.PitchShiftEffect(1.5);
	b.Save("b.wav");
  
	return 0;
}
