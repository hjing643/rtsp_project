#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int client_socket = -1;

// 打印错误信息
void print_error(const char* message, int error_code) {
    char error_buffer[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(error_code, error_buffer, sizeof(error_buffer));
    fprintf(stderr, "%s: %s\n", message, error_buffer);
}

int ConnectToServer(char* ptr_ip, int port)
{
    if (client_socket >=0 )
    {
        return client_socket;
    }
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

     if (client_socket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置服务器地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ptr_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(client_socket);
        client_socket = -1;
        return -1;
    }

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        client_socket = -1;
        return -1;
    }
    return client_socket;
}

void SendRTPStream(const char* ptr_data, int data_len)
{
    if(ConnectToServer("10.199.28.221", 8888) < 0)
    {
        return;
    }
     // 发送数据
    int bytes_sent = send(client_socket, ptr_data, data_len, 0);
    if (bytes_sent < 0) {
        perror("Send failed");
        return;
    }
    const char* ptr_end = "huangjingyf";
    int bytes_sent2 = send(client_socket, ptr_end, strlen(ptr_end), 0);

    printf("Sent len: %d-%d\n",data_len + strlen(ptr_end), bytes_sent + bytes_sent2);
}


// 解析 NALU
void parse_nalu(const uint8_t* data, int size) {
     const uint8_t* end = data + size;
    while (data < end) {
        // 查找起始码
        const uint8_t* data_start = data;

        const uint8_t* nalu_start = NULL;
        int add = 0;
        if (data + 4 <= end && data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x00 && data[3] == 0x01) {
            data_start = data;
            nalu_start = data + 4;  // 4 字节起始码
            add = 4;
            data += 4;
        } else if (data + 3 <= end && data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x01) {
             data_start = data;
            nalu_start = data + 3;  // 3 字节起始码
            add = 3;
            data += 3;
        } else {
            data++;
            continue;
        }

         // 查找下一个起始码
        const uint8_t* nalu_end = data;
        while (nalu_end + 4 <= end && !(nalu_end[0] == 0x00 && nalu_end[1] == 0x00 && nalu_end[2] == 0x00 && nalu_end[3] == 0x01)) 
        {
            nalu_end++;
        }
        while (nalu_end + 3 <= end && !(nalu_end[0] == 0x00 && nalu_end[1] == 0x00 && nalu_end[2] == 0x01)) 
        {
            nalu_end++;
        }

        // 计算 NALU 长度
        int nalu_length = nalu_end - nalu_start;
        if (nalu_length <= 0) {
            break;
        }

        SendRTPStream(data_start, nalu_length + add);

        // 解析 NALU 头
        uint8_t nal_unit_type = nalu_start[0] & 0x1F;  // 取低 5 位
        //printf("NALU Type: ");
        //print_nalu_type(nal_unit_type);
        //printf("NALU Length: %d\n", nalu_length);

        // 跳过当前 NALU
        data = nalu_end;
    }
}

// 自定义 IO 回调函数
static int write_packet(void* opaque, uint8_t* buf, int buf_size) {
    // 这里可以将数据发送到网络或其他地方
    printf("Received %d bytes of data\n", buf_size);
    
    SendRTPStream(buf, buf_size);

    return buf_size;  // 返回实际处理的字节数
}

int main() {

    const char* rtsp_url = "rtsp://admin:Vmspro135@10.20.103.98:554/cam/realmonitor?channel=1&subtype=0";
     AVFormatContext* input_ctx = NULL;
    AVFormatContext* output_ctx = NULL;
    AVPacket packet;
    int ret;

    // 初始化 FFmpeg 库
    av_register_all();
    avformat_network_init();

    // 设置 RTSP 传输协议为 TCP
    AVDictionary* options = NULL;
    av_dict_set(&options, "rtsp_transport", "tcp", 0);

    // 打开输入流
    ret = avformat_open_input(&input_ctx, rtsp_url, NULL, &options);
    if (ret < 0) {
        fprintf(stderr, "无法打开输入流\n");
        return -1;
    }

    // 获取流信息
    if (avformat_find_stream_info(input_ctx, NULL) < 0) {
        fprintf(stderr, "无法获取流信息\n");
        avformat_close_input(&input_ctx);
        return -1;
    }

    // 打印输入流信息
    av_dump_format(input_ctx, 0, rtsp_url, 0);

    // 创建输出上下文
    avformat_alloc_output_context2(&output_ctx, NULL, "mp4", NULL);
    if (!output_ctx) {
        fprintf(stderr, "无法创建输出上下文\n");
        avformat_close_input(&input_ctx);
        return -1;
    }

    // 复制流信息
    int i = 0;
    for (i = 0; i < input_ctx->nb_streams; i++) {
        AVStream* in_stream = input_ctx->streams[i];
        AVStream* out_stream = avformat_new_stream(output_ctx, NULL);
        if (!out_stream) {
            fprintf(stderr, "无法创建输出流\n");
            avformat_close_input(&input_ctx);
            avformat_free_context(output_ctx);
            return -1;
        }
        avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
        out_stream->codecpar->codec_tag = 0;  // 避免编码器标签冲突
    }

    // 创建自定义 IO 上下文
    uint8_t* io_buffer = (uint8_t*)av_malloc(4096);  // 缓冲区
    AVIOContext* avio_ctx = avio_alloc_context(
        io_buffer, 4096, 1, NULL, NULL, write_packet, NULL);
    if (!avio_ctx) {
        fprintf(stderr, "无法创建自定义 IO 上下文\n");
        avformat_close_input(&input_ctx);
        avformat_free_context(output_ctx);
        return -1;
    }
    output_ctx->pb = avio_ctx;  // 将自定义 IO 上下文赋值给输出上下文

    // 写入文件头
    ret = avformat_write_header(output_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "无法写入文件头\n");
        avformat_close_input(&input_ctx);
        avformat_free_context(output_ctx);
        return -1;
    }

    // 读取数据包并写入输出
    while (av_read_frame(input_ctx, &packet) >= 0) {
        AVStream* in_stream = input_ctx->streams[packet.stream_index];
        AVStream* out_stream = output_ctx->streams[packet.stream_index];

        // 调整时间戳
        packet.pts = av_rescale_q_rnd(packet.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
        packet.dts = av_rescale_q_rnd(packet.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
        packet.duration = av_rescale_q(packet.duration, in_stream->time_base, out_stream->time_base);
        packet.pos = -1;

        // 写入数据包
        ret = av_interleaved_write_frame(output_ctx, &packet);
        if (ret < 0) {
            fprintf(stderr, "写入数据包失败\n");
            break;
        }

        av_packet_unref(&packet);
    }

    // 写入文件尾
    av_write_trailer(output_ctx);

    // 释放资源
    avformat_close_input(&input_ctx);
    if (output_ctx && !(output_ctx->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&output_ctx->pb);
    }
    avformat_free_context(output_ctx);
    av_dict_free(&options);

    printf("RTSP 流转封装完成\n");
    return 0;
}
