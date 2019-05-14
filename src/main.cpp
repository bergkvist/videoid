#include <iostream>
#include "video_comparison.hpp"
#include "stopwatch.hpp"
/*
int main (int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: compare-videos [asset id] [compilation id]";
        return -1;
    }

    if (VERBOSE) {
        std::cout << "OpenCV version: " << CV_VERSION << std::endl;
        std::cout << "HASH_SIZE=" << HASH_SIZE << " (output file: " << OUTPUT << ")" << std::endl;
    }

    std::ofstream csv{OUTPUT};
    stopwatch t1, t2;

    t1.start();
    ContentID::HashedVideo asset_video{argv[1]}; // asset
    ContentID::HashedVideo compilation_video{argv[2]}; // compilation
    t1.stop();

    t2.start();
    ContentID::VideoComparison comparison{asset_video, compilation_video};
    csv << "raw,avg,std,bin,min_avg,max_std\n"; 
    for (size_t i = 0; i < comparison.signal.raw.size(); ++i) {
        csv << comparison.signal.raw[i];
        if (i > WINDOW_SIZE/2 && i < comparison.signal.raw.size() - WINDOW_SIZE/2) {
            csv << "," << comparison.signal.moving_avg[i-WINDOW_SIZE/2]
                << "," << comparison.signal.moving_std[i-WINDOW_SIZE/2]
                << "," << comparison.signal.binary[i-WINDOW_SIZE/2];
        } else {
            csv << ",,,";
        }
        csv << "," << comparison.signal.min_avg << "," << comparison.signal.max_std << "\n";
    }
    t2.stop();

    if(VERBOSE) std::cout 
        << "\nElapsed time:\n hash: " 
        << t1.elapsed_time() 
        << " s\n comp: "
        << t2.elapsed_time()
        << " s\n";

    return 0;
}*/

cv::Size get_square_size(cv::VideoCapture video) {
    //const int w = video.get(cv::CAP_PROP_FRAME_WIDTH);
    const int h = video.get(cv::CAP_PROP_FRAME_HEIGHT);
    return cv::Size{h, h};
}

int get_threshold(cv::Mat gray_frame) {
    long int threshold = 0;
    for (size_t i = 0; i < BITS_IN_UINT * HASH_LENGTH; ++i)
        threshold += (long int) gray_frame.data[i];
    threshold /= (BITS_IN_UINT * HASH_LENGTH);
    return threshold;
}

void create_video(cv::VideoCapture video) {
    const auto size = get_square_size(video);
    const auto fps = video.get(cv::CAP_PROP_FPS);
    const auto fourcc = (int)video.get(cv::CAP_PROP_FOURCC);
    const auto hash_size = cv::Size{HASH_SIZE, HASH_SIZE};
    const auto outfile = std::string{"./images/out.mp4"};

    auto videow = cv::VideoWriter{outfile, fourcc, fps, size, false};
    auto frame = cv::Mat{};
    while(video.read(frame)) {
        cv::resize(frame, frame, hash_size);
        cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);

        int threshold = get_threshold(frame);
        for (size_t i = 0; i < HASH_SIZE*HASH_SIZE; ++i)
            frame.data[i] = (frame.data[i] > threshold) ? 255 : 0;

        cv::resize(frame, frame, size);
        videow.write(frame);
    }   
}

cv::Mat hash_frame(cv::Mat frame) {
    const cv::Size hash_size(HASH_SIZE, HASH_SIZE);
    auto new_frame = cv::Mat{};

    cv::resize(frame, new_frame, hash_size);
    cv::cvtColor(new_frame, new_frame, cv::COLOR_RGB2GRAY);

    int threshold = get_threshold(new_frame);
    for (size_t i = 0; i < HASH_SIZE*HASH_SIZE; ++i)
    new_frame.data[i] = (new_frame.data[i] > threshold) ? 255 : 128;
    return new_frame;
}

int main(int argc, char** argv) {
    create_video(cv::VideoCapture{"./videos/j4hnIAqyhGE.mp4"});
    /*
    const auto out = std::string{"./images/335"};
    auto video1 = cv::VideoCapture{"./videos/ZTHsrEG5jhA.mp4"};
    auto video2 = cv::VideoCapture{"./videos/j4hnIAqyhGE.mp4"};
    //const double framerate = video.get(cv::CAP_PROP_FPS);
    const size_t frame_count_1 = video1.get(cv::CAP_PROP_FRAME_COUNT);
    const size_t frame_count_2 = video2.get(cv::CAP_PROP_FRAME_COUNT);

    auto f = cv::Mat{};
    auto frame1_in = cv::Mat{};
    auto frame2_in = cv::Mat{};
    auto frame3_in = cv::Mat{};
    
    for (size_t i = 0; i < frame_count_1; ++i) {
        if      (i == 330) video1.read(frame1_in);
        else if (i == 335) video1.read(frame2_in);
        //else if (i == 200) video.read(frame3_in);
        else video1.read(f);
    }

    for (size_t i = 0; i < frame_count_2; ++i) {
        if      (i == 1000) video2.read(frame3_in);
        else video2.read(f);
    }

    auto frame1_out = hash_frame(frame1_in);
    auto frame2_out = hash_frame(frame2_in);
    auto frame3_out = hash_frame(frame3_in);
    auto frame_comp_1_2 = frame1_out.clone();
    auto frame_over_1_2 = frame1_out.clone();
    auto frame_comp_1_3 = frame1_out.clone();
    auto frame_over_1_3 = frame1_out.clone();

    
    int diff_1_2_count = 0;
    int diff_1_3_count = 0;
    for (size_t i = 0; i < HASH_SIZE*HASH_SIZE; ++i) {
        frame_comp_1_2.data[i] = frame1_out.data[i] ^ frame2_out.data[i];
        frame_comp_1_3.data[i] = frame1_out.data[i] ^ frame3_out.data[i];
        frame_over_1_2.data[i] = 255-((255-frame1_out.data[i]) + (255-frame2_out.data[i]));
        frame_over_1_3.data[i] = 255-((255-frame1_out.data[i]) + (255-frame3_out.data[i]));
        if (frame_comp_1_2.data[i] > 0) diff_1_2_count++;
        if (frame_comp_1_3.data[i] > 0) diff_1_3_count++;
    }


    std::cout << "# of pixels:\t" << HASH_SIZE * HASH_SIZE << std::endl;

    std::cout << "\ndiff_1_2_count\t" << diff_1_2_count << std::endl;
    std::cout << "similarity(1,2)\t" << 1 - diff_1_2_count * 1.0 / (HASH_SIZE * HASH_SIZE) << std::endl;
    
    std::cout << "\ndiff_1_3_count\t" << diff_1_3_count << std::endl;
    std::cout << "similarity(1,3)\t" << 1 - diff_1_3_count * 1.0 / (HASH_SIZE * HASH_SIZE) << std::endl;
    

    cv::imwrite("./images/frame1_in.jpg", frame1_in);
    cv::imwrite("./images/frame2_in.jpg", frame2_in);
    cv::imwrite("./images/frame3_in.jpg", frame3_in);
    cv::imwrite("./images/frame1_out.jpg", frame1_out);
    cv::imwrite("./images/frame2_out.jpg", frame2_out);
    cv::imwrite("./images/frame3_out.jpg", frame3_out);
    cv::imwrite("./images/frame_comp_1_2.jpg", frame_comp_1_2);
    cv::imwrite("./images/frame_over_1_2.jpg", frame_over_1_2);
    cv::imwrite("./images/frame_comp_1_3.jpg", frame_comp_1_3);
    cv::imwrite("./images/frame_over_1_3.jpg", frame_over_1_3);
    */
    /*
    cv::imwrite(out + "-original.jpg", frame);

    
    cv::resize(frame, frame, hash_size);
    cv::imwrite(out + "-resized.jpg", frame);

    cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
    cv::imwrite(out + "-grayscale.jpg", frame);

    long int threshold = 0;
    for (size_t i = 0; i < BITS_IN_UINT * HASH_LENGTH; ++i)
        threshold += (long int) frame.data[i];
    threshold /= (BITS_IN_UINT * HASH_LENGTH);

    for (size_t i = 0; i < HASH_SIZE*HASH_SIZE; ++i)
    frame.data[i] = (frame.data[i] > threshold) ? 255 : 0;

    cv::imwrite(out + "-binary.jpg", frame);*/
}

