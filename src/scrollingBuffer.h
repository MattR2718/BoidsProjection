#ifndef SCROLLINGBUFFER_h
#define SCROLLINGBUFFER_h

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    float avg[1] = {0.0};
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x,y));
        else {
            Data[Offset] = ImVec2(x,y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset  = 0;
        }
    }
    void getAvg(){
        int t = 0;
        for(auto& v : Data){
            t += v.y;
        }
        avg[0] = static_cast<float>(t) / static_cast<float>(Data.size());
    }
};

#endif