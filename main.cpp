#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat src;
cv::Mat undistorted;

double focal = 10'000;

bool show_orig = true;

std::string const name = "image";

double target_resize = 1.0;

void show() {
    if (show_orig) {
        cv::imshow(name, src);
        return;
    }
    // Principal point of the source image (simply the center since we don't have calibration data and it's good enough).
    cv::Vec2f const src_pc(double(src.size().width+1)/2, double(src.size().height+1)/2);

    cv::Mat_<cv::Vec2f> map(src.rows*target_resize, src.cols*target_resize);

    // Principal point of the map / destination image, also just the center.
    cv::Vec2f const dst_pc(double(map.size().width+1)/2, double(map.size().height+1)/2);

    // Fill the map which tells the remap function for each pixel in the undistorted image where it comes from in the source fisheye image.
    for (int ii = 0; ii < map.rows; ++ii) {
        for (int jj = 0; jj < map.cols; ++jj) {
            cv::Vec2f &px = map(ii, jj);
            px = cv::Vec2f(jj, ii);
            px -= dst_pc;
            px /= focal;
            double const r = cv::norm(px);
            px *= std::atan(r)/r;
            px *= focal;
            px += src_pc;
        }
    }

    cv::remap(src, undistorted, map, cv::noArray(), cv::INTER_LINEAR, cv::BORDER_REPLICATE);

    cv::imshow(name, undistorted);
    std::cout << "Undistortion done, focal: " << focal << ", resize: " << target_resize << std::endl;
}

int main(int argc, char ** argv) {

    std::cout << "Usage: " << argv[0] << " <input image file>" << std::endl
              << "The program initially shows the original image." << std::endl
              << "Use the following keys:" << std::endl
              << "+/-: for increasing/decreasing the focal length used for undistorting the image." << std::endl
              << "       Decreasing the focal length increases the effect of the undistortion." << std::endl
              << "9/*: to increase/decrease the size of the undistorted image" << std::endl
              << "s  : to save the undistorted image as jpg" << std::endl
              << "q  : quit";

    if (argc < 2) {
        std::cout << "Input image required" << std::endl;
        return EXIT_FAILURE;
    }

    src = cv::imread(argv[1], cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

    show();
    while(true) {
        char key = cv::waitKey(0);
        std::cout << "key: " << key << std::endl;
        switch(key) {
        case 'q': return EXIT_SUCCESS;
        case '+': focal *= 1.1; show_orig = false; show(); break;
        case '-': focal /= 1.1; show_orig = false; show(); break;
        case '9': target_resize *= 1.1; show_orig = false; show(); break;
        case '*': target_resize /= 1.1; show_orig = false; show(); break;
        case 's': cv::imwrite(std::string(argv[1]) + "-undistorted.jpg", undistorted, {cv::IMWRITE_JPEG_QUALITY, 95}); break;
        }
    }

    return 0;
}
