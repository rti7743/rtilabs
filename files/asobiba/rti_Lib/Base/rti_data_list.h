
class TCommandList
{
    char **CL;

    public:
        //コマンドリスト設定.
        void set(char **C){CL = C;};
        //サーチ
        int search(char *str,char **NextChar);
};
