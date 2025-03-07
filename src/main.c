#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <stdio.h>
#include <stdlib.h>


// 打印错误信息
void print_error(const char* message, int error_code) {
    char error_buffer[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(error_code, error_buffer, sizeof(error_buffer));
    fprintf(stderr, "%s: %s\n", message, error_buffer);
}

int main() {

	printf("FFmpeg version: %s\n", av_version_info());
	av_register_all();
    	const char* rtsp_url = "rtsp://admin:Vmspro135@10.20.103.98:554/cam/realmonitor?channel=1&subtype=0";
	//const char* rtsp_url ="rtsp://admin:12345@10.199.13.41:8888/stream";  
 const char* output_file = "./output1.h264";

    AVFormatContext* format_ctx = NULL;
    AVPacket packet;
    FILE* output_fp = NULL;

    // 初始化 FFmpeg 网络库
    if (avformat_network_init() < 0)
	{
		print_error("init network failed", -1);
		return -1;
	}


//AVDictionary* options = NULL;
//av_dict_set(&options, "rtsp_transport", "tcp", 0);  // 强制使用 TCP 传输

    // 打开输入流
    int ret = avformat_open_input(&format_ctx, rtsp_url, NULL, NULL);
if (ret < 0) {
	printf("open input failed:%d,%s\n", ret,rtsp_url);
        print_error("无法打开输入流", ret);
        return -1;
    }

    // 获取流信息
    if (avformat_find_stream_info(format_ctx, NULL) < 0) {
        print_error("无法获取流信息", -1);
        avformat_close_input(&format_ctx);
        return -1;
    }

    // 打印流信息
    av_dump_format(format_ctx, 0, rtsp_url, 0);

    // 打开输出文件
    output_fp = fopen(output_file, "wb");
    if (!output_fp) {
        perror("无法打开输出文件");
        avformat_close_input(&format_ctx);
        return -1;
    }

    // 读取数据包并写入文件
    while (av_read_frame(format_ctx, &packet) >= 0) {
        fwrite(packet.data, 1, packet.size, output_fp);
        av_packet_unref(&packet);
    }

    // 清理资源
    fclose(output_fp);
    avformat_close_input(&format_ctx);
    avformat_network_deinit();

    printf("取流完成，数据已保存到 %s\n", output_file);
    return 0;
}
