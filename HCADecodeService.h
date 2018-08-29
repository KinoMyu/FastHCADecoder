﻿#pragma once
#ifndef HCADECODESERVICE_H
#define HCADECODESERVICE_H

#include <thread>
#include <deque>
#include <map>
#include <mutex>
#include "Semaphore.h"
#include "clHCA.h"

class HCADecodeService
{
public:
    HCADecodeService();
    HCADecodeService(unsigned int num_threads, unsigned int chunksize = 24);
    ~HCADecodeService();
    void cancel_decode(void *ptr);
    std::pair<void*, size_t> decode(const char *hcafilename, unsigned int decodefromsample = 0, unsigned int ciphKey1 = 0xBC731A85, unsigned int ciphKey2 = 0x0002B875, float volume = 1.0f, int mode = 16, int loop = 0);
    void wait_on_request(void *ptr);
    void wait_for_finish();
private:
    void Main_Thread();
    void Decode_Thread(int id);
    void load_next_request();
    void populate_block_list();
    void wait_on_all_threads(Semaphore &sem);
    void join_workers();

    clHCA workingfile;
    unsigned int numthreads, numchannels, chunksize, startingblock;
    void *workingrequest;
    std::thread dispatchthread;
    std::thread *workerthreads;
    std::map<void*, std::pair<clHCA, unsigned int>> filelist;
    std::deque<unsigned int> blocks;
    int *workingblocks;
    Semaphore *workersem;
    Semaphore mainsem, datasem, wavoutsem;
    std::mutex filelistmtx, workingmtx;
    clHCA::stChannel *channels;
    bool shutdown, stopcurrent;
};

#endif //HCADECODESERVICE_H