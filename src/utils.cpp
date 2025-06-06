#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// PSNR and SSIM implementations comes from
// https://docs.opencv.org/4.x/dd/d3d/tutorial_gpu_basics_similarity.html

double PSNR(const cv::Mat& I1, const cv::Mat& I2) {
    cv::Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2
 
    cv::Scalar s = sum(s1);         // sum elements per channel
 
    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
 
    if ( sse <= 1e-10) { // for small values return zero
        return 0;

    } else {
        double  mse =sse /(double)(I1.channels() * I1.total());
        double psnr = 10.0*log10((255*255)/mse);
        return psnr;
    }
}

cv::Scalar SSIM(const cv::Mat& i1, const cv::Mat& i2) {
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d     = CV_32F;
 
    cv::Mat I1, I2;
    i1.convertTo(I1, d);           // cannot calculate on one byte large values
    i2.convertTo(I2, d);
 
    cv::Mat I2_2   = I2.mul(I2);        // I2^2
    cv::Mat I1_2   = I1.mul(I1);        // I1^2
    cv::Mat I1_I2  = I1.mul(I2);        // I1 * I2
 
    /*************************** END INITS **********************************/
 
    cv::Mat mu1, mu2;   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);
 
    cv::Mat mu1_2   =   mu1.mul(mu1);
    cv::Mat mu2_2   =   mu2.mul(mu2);
    cv::Mat mu1_mu2 =   mu1.mul(mu2);
 
    cv::Mat sigma1_2, sigma2_2, sigma12;
 
    GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;
 
    GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
 
    GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;
 
    cv::Mat t1, t2, t3;
 
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
 
    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))
 
    cv::Mat ssim_map;
    divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;
 
    cv::Scalar mssim = mean( ssim_map ); // mssim = average of ssim map
    return mssim;
}

double SSIM_lum(const cv::Mat& i1, const cv::Mat& i2){
    cv::Mat I1_Lab, I2_Lab;
    cv::Mat I1_L, I2_L;

    cv::cvtColor(i1, I1_Lab, cv::COLOR_BGR2Lab);
    cv::cvtColor(i2, I2_Lab, cv::COLOR_BGR2Lab);

    // Seulement sur la Luminance
    cv::extractChannel(I1_Lab, I1_L, 0);
    cv::extractChannel(I2_Lab, I2_L, 0);

    cv::Scalar ssim_l = SSIM(I1_L,I2_L);
    return ssim_l[0];
} 