class WebSocketFrame {
  public: 
    explicit WebSocketFrame();
  private:
    bool _fin;
    bool _maskBit;
    bool _mask; // 32 bits
};