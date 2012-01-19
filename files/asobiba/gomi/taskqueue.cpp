/*
taskqueue
やりたいこと:タスクキューを作りたい。
タスクスレッドが動いているので、そいつにタスクを lambda(関数オブジェクト)で与えると、どんどん実行してくれるみたいな。

やって欲しいタスクをメッセージではなくて、lambda で与えるのが特徴？
C#の Invoke() みたいなノリで動くタスクキューがほしい。


で、とりあえず作ったけど、
なぜか、タスクをキューに積んでいる状態で、スレッドを止めようとすると停止フラグを無視してフリーズするっぽい。
何でだろう？
boost::unique_future先生が変なことしてるのかな？

*/
#include <iostream>
#include <boost/thread.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/move/move.hpp>
#include <boost/bind.hpp>
#include <deque>
//#include <functional>

//http://stackoverflow.com/questions/5889532/how-do-i-create-a-queue-that-holds-boostpackaged-task-with-functions-that-re
template<typename Functor> class taskqueue
{
public:
	typedef typename boost::result_of<Functor()>::type result_type;

//	typedef int result_type;
public:
	//キューにタスクを積む
	boost::unique_future<result_type> push_async(Functor functor)
	{
		boost::packaged_task<result_type> task(functor);
		boost::unique_future<result_type> future = task.get_future();
		{
			boost::unique_lock<boost::mutex> al(this->lock);
			internal_queue.push_back(boost::move(task));
//やるとコンパイルエラーに・・・ まずはやらないですすめる.
//			//boost::shared_ptr is possibly move-enabled so you can try moving it
//			internal_queue.push_back( boost::bind(dereference_functor(), task) );
		}
		return boost::move(future);
	}
	//キューにタスクを積んで即実行 SendMessageみたいなのりで
	result_type push_sync(Functor functor)
	{
		boost::unique_future<ret_type> r = push_future(Functor functor);
		return r.get();
	}

	//スレッドが呼び出す。
	//タスクキューを見て、タスクがあれば実行する。
	//なければ false を返して何もしない.
	bool execute()
	{
		boost::packaged_task<result_type> task;
		{
			boost::unique_lock<boost::mutex> al(this->lock);
			if (internal_queue.empty() )
			{
				return false;
			}
			task = boost::move( internal_queue.front() );
			internal_queue.pop_front();
		}
		task();
		return true;
	}

	std::deque< boost::packaged_task<result_type> > internal_queue;
	boost::mutex lock;
	boost::condition_variable queue_wait;
};

class threadFunc
{
public:
//	int operator()()
	int main()
	{
		boost::this_thread::interruption_enabled();
		while(1)
		{
			boost::this_thread::interruption_point();
			queue.execute();
		}
		return 0;
	}

	void append_task(int arg1)
	{
		queue.push_async(
			[arg1]() -> int {
				std::cout << "Task スタート! Number:" << arg1 << std::endl;
				boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(2000));

				std::cout << "Task 終了! Number:" << arg1 << std::endl;
				return 0; 
			}
		);
	}

	taskqueue< boost::function<int (void)> > queue;
};

int main() 
{
	threadFunc obj;
	boost::thread th(boost::bind(&threadFunc::main, &obj));

	int task_number = 0;

	while(1)
	{
		std::cout << "コマンドを入れて エンターキーおしてください。(q:終了 / a:タスクの追加) " << std::endl;
		std::string str;
		getline( std::cin, str );
		if (str == "q")
		{
			break;
		}
		if (str == "a")
		{
			obj.append_task(task_number);
			std::cout << "task " << task_number << " を登録しました。" << std::endl;
			task_number++;
		}

	}
	std::cout << " ( ﾟдﾟ)ﾎﾟｶｰﾝ" << std::endl;
	std::cout << "interrupt 停止命令送信" << std::endl;
	th.interrupt();
	std::cout << "join 停止待ち" << std::endl;
	th.join();
	std::cout << "プログラムを終了します" << std::endl;
	return 0;
}
