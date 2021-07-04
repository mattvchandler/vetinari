// extracted from Ticking Clock" by AntumDeluge CC0 (https://freesound.org/people/AntumDeluge/sounds/188033/)
const unsigned char tick_pcm[] = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x02, 0x00, 0xfd, 0xff, 0x01, 0x00,
  0xfd, 0xff, 0x04, 0x00, 0xfa, 0xff, 0x08, 0x00, 0xdb, 0xff, 0x93, 0xff,
  0xaf, 0xff, 0x6f, 0xfe, 0x7a, 0xfd, 0x9b, 0xfc, 0x22, 0xfc, 0x8c, 0xfc,
  0xfe, 0xfb, 0xe8, 0xfb, 0xc8, 0xfc, 0x5d, 0xfd, 0xf3, 0xfc, 0x40, 0xfc,
  0x28, 0xfc, 0x8f, 0xfc, 0xe6, 0xfc, 0x2a, 0xfc, 0xbc, 0xfb, 0xe9, 0xfc,
  0x94, 0xfd, 0xd9, 0xfd, 0xfe, 0xfd, 0xa2, 0xfd, 0x0f, 0xfe, 0x08, 0xfe,
  0x10, 0xfe, 0x1b, 0xfe, 0xc4, 0xfe, 0x8e, 0xfe, 0x71, 0xfd, 0x87, 0xfd,
  0x7a, 0xfe, 0x15, 0xff, 0xd0, 0xfe, 0x19, 0xff, 0x9f, 0xff, 0xf0, 0x00,
  0xd6, 0xff, 0x90, 0xfe, 0x05, 0x00, 0x36, 0xff, 0xd3, 0xfe, 0xf2, 0xfd,
  0x97, 0xff, 0x47, 0x00, 0x32, 0x00, 0x41, 0xff, 0xe0, 0xfe, 0x1b, 0xff,
  0xea, 0xfd, 0x15, 0xff, 0x56, 0xfd, 0x13, 0xfe, 0xee, 0xfe, 0x4f, 0xff,
  0x34, 0xff, 0xc4, 0xfd, 0x75, 0xfe, 0x40, 0xff, 0x54, 0xff, 0x0c, 0xff,
  0xfd, 0xfd, 0x3c, 0xff, 0x9e, 0x00, 0x10, 0xff, 0xeb, 0xfe, 0x11, 0xff,
  0xb0, 0x01, 0x80, 0x02, 0xa3, 0xff, 0x2f, 0xfd, 0xcb, 0xfd, 0xd5, 0x00,
  0x43, 0x01, 0xc1, 0xff, 0x0b, 0xfe, 0x88, 0xfe, 0x14, 0x01, 0x31, 0x02,
  0xb2, 0x03, 0x92, 0x03, 0xae, 0x02, 0x19, 0x02, 0xf2, 0x00, 0x8e, 0x01,
  0xec, 0x01, 0x9c, 0x01, 0x02, 0x01, 0x4a, 0x01, 0xde, 0x00, 0x2f, 0x00,
  0x6b, 0x00, 0xcb, 0xff, 0x3b, 0xfe, 0x47, 0xfd, 0x4a, 0xfe, 0xc9, 0xff,
  0x92, 0x00, 0x69, 0xff, 0x5b, 0xfd, 0xb7, 0xfd, 0x19, 0xfe, 0x20, 0xff,
  0xbc, 0xff, 0x92, 0xff, 0x28, 0xfd, 0xee, 0xfd, 0x5f, 0x00, 0x9d, 0x00,
  0x22, 0x00, 0xf7, 0xfd, 0x5e, 0xff, 0x65, 0x00, 0xfa, 0x00, 0xe3, 0xfe,
  0x1e, 0xfe, 0xe0, 0xfd, 0x33, 0xff, 0x16, 0x00, 0x6c, 0x00, 0x3a, 0xff,
  0x71, 0xfd, 0x69, 0xfd, 0x2f, 0xfe, 0x25, 0x00, 0xc8, 0xff, 0x71, 0xfd,
  0x7a, 0xfc, 0xf5, 0xfc, 0xc4, 0xfd, 0x05, 0x00, 0x68, 0x02, 0x5c, 0x01,
  0xc5, 0xfe, 0x3b, 0xfd, 0xda, 0xfc, 0x3e, 0xff, 0x97, 0xff, 0x79, 0xfe,
  0xed, 0xfb, 0x5a, 0xfc, 0x16, 0xfe, 0x1b, 0xff, 0xb2, 0xfe, 0x27, 0xfd,
  0x04, 0xfe, 0x7c, 0xfe, 0xbd, 0xff, 0x12, 0xff, 0x86, 0xfd, 0xe5, 0xfc,
  0x7a, 0xfd, 0xcd, 0xfe, 0xae, 0xff, 0xe1, 0xff, 0x45, 0xfe, 0x6b, 0xfd,
  0xc9, 0xfe, 0xb4, 0x00, 0xb6, 0x00, 0x5f, 0xff, 0xb1, 0xfe, 0xec, 0xfe,
  0xfa, 0xff, 0x56, 0x00, 0x97, 0xff, 0x88, 0xfe, 0x1e, 0xfe, 0x86, 0xfc,
  0x37, 0xfe, 0xa4, 0xff, 0x64, 0xff, 0x9b, 0xfe, 0xff, 0xfb, 0xb1, 0x01,
  0x37, 0xfd, 0x49, 0xfd, 0xf6, 0xfd, 0x14, 0xfe, 0x6b, 0x09, 0xba, 0xf9,
  0xc5, 0x03, 0x48, 0xfb, 0x67, 0x05, 0x91, 0xf9, 0xf6, 0xff, 0x63, 0x0c,
  0xf7, 0xfb, 0x5b, 0x09, 0x8a, 0xed, 0x1f, 0x09, 0x38, 0x0a, 0xb8, 0x03,
  0x0e, 0xff, 0x0f, 0x04, 0xd8, 0x0b, 0x30, 0xfe, 0xc0, 0xf0, 0x4d, 0xf6,
  0xf0, 0xf3, 0x62, 0xfd, 0x5c, 0x08, 0x0f, 0x1b, 0xbd, 0x02, 0xe3, 0xf9,
  0x1f, 0xf3, 0xdd, 0xec, 0xd1, 0x08, 0x37, 0x05, 0xa5, 0xff, 0x19, 0x03,
  0xa7, 0xf9, 0x31, 0xf7, 0xd5, 0x0f, 0x47, 0xfb, 0x3f, 0x00, 0xb9, 0xfd,
  0x07, 0xf6, 0x58, 0x00, 0x39, 0xf5, 0x15, 0xff, 0xa7, 0x0c, 0x75, 0x04,
  0x1d, 0xfb, 0xfe, 0xf3, 0xa3, 0x06, 0xb5, 0xf8, 0xa2, 0xef, 0xb9, 0xfb,
  0x02, 0x08, 0xa0, 0x1c, 0x62, 0x09, 0x48, 0xf4, 0x03, 0xf7, 0x66, 0xf2,
  0x85, 0xfd, 0x29, 0x04, 0x91, 0x05, 0x27, 0x05, 0x1e, 0xfa, 0x0c, 0xf7,
  0xcd, 0x0a, 0x74, 0x08, 0xab, 0x02, 0x0c, 0xf3, 0x68, 0xf4, 0x36, 0x08,
  0x07, 0x12, 0x21, 0x0f, 0xf9, 0xfb, 0xce, 0xee, 0xf6, 0xee, 0xef, 0xf7,
  0xeb, 0x02, 0x56, 0xfb, 0x19, 0x03, 0xbf, 0x03, 0xae, 0xf9, 0xd1, 0xfb,
  0x69, 0xfd, 0xa5, 0xff, 0x16, 0x01, 0xe1, 0xf4, 0xb5, 0xf1, 0xf3, 0xfc,
  0xbe, 0xff, 0xec, 0xfd, 0x3b, 0xfd, 0xa1, 0xf9, 0x0b, 0xfd, 0xfe, 0x02,
  0xc5, 0x04, 0xaf, 0x00, 0x9d, 0xf6, 0xf9, 0xf5, 0x18, 0xfe, 0x24, 0x01,
  0xe8, 0xff, 0xe5, 0xfd, 0xaf, 0xfc, 0xb1, 0x00, 0x78, 0x05, 0xdb, 0x03,
  0x41, 0x04, 0xfc, 0x02, 0x32, 0x01, 0xc4, 0x03, 0x23, 0x01, 0xc6, 0x00,
  0x01, 0x04, 0x09, 0x05, 0x6f, 0x03, 0xc0, 0xfe, 0xa6, 0xfd, 0x55, 0xfe,
  0x64, 0x03, 0x44, 0x01, 0x29, 0x00, 0x07, 0x03, 0x4e, 0x00, 0xbc, 0x01,
  0x03, 0x04, 0x96, 0x01, 0xa4, 0xfe, 0x0d, 0xfe, 0xee, 0xff, 0xba, 0x06,
  0xfb, 0x08, 0xdc, 0x00, 0x38, 0x00, 0x2d, 0x00, 0x29, 0xfe, 0xcc, 0xfc,
  0x31, 0xf9, 0xfe, 0xfb, 0xaa, 0x00, 0x2e, 0x04, 0x4b, 0x06, 0xf3, 0x02,
  0xfa, 0xfd, 0x94, 0xfa, 0x66, 0xfc, 0x36, 0xfd, 0x18, 0x00, 0xfa, 0xfe,
  0x04, 0xfd, 0x64, 0xfe, 0xb9, 0xfd, 0x02, 0xfe, 0xd8, 0xfe, 0xf3, 0xfd,
  0x8a, 0xfc, 0x5f, 0xfb, 0x5f, 0xfc, 0x3d, 0xff, 0x2b, 0xfe, 0x14, 0xfc,
  0x66, 0xfd, 0x24, 0xfc, 0xf0, 0xf8, 0xef, 0xfa, 0x49, 0xfc, 0x11, 0xff,
  0x10, 0x01, 0x55, 0x01, 0x12, 0xff, 0x7e, 0xfb, 0x84, 0xf9, 0x20, 0xfb,
  0x3d, 0xfc, 0x0c, 0xfa, 0xb4, 0xfb, 0x10, 0xfe, 0x6c, 0xff, 0x2d, 0x00,
  0x7e, 0xfd, 0x1d, 0xfc, 0x92, 0xfc, 0x6a, 0xfc, 0x99, 0xfc, 0x59, 0xfc,
  0x97, 0xfb, 0x19, 0xfe, 0xd7, 0x00, 0xde, 0x00, 0x8c, 0xff, 0x1d, 0xfc,
  0x02, 0xfc, 0xc7, 0x00, 0x70, 0x03, 0x9d, 0x04, 0xde, 0x03, 0x33, 0x01,
  0x5f, 0x00, 0x76, 0x01, 0xc6, 0x03, 0xe0, 0x05, 0x84, 0x03, 0x59, 0x01,
  0xb9, 0x00, 0x83, 0x00, 0x52, 0x02, 0xcf, 0x05, 0xc9, 0x04, 0x37, 0x01,
  0xf5, 0xff, 0x78, 0x00, 0x3f, 0x03, 0xf8, 0x03, 0x92, 0x02, 0xe5, 0x02,
  0xfa, 0x02, 0xf7, 0x02, 0x8d, 0x02, 0x4e, 0x02, 0xcd, 0x00, 0xd2, 0xfe,
  0xb4, 0xfd, 0x85, 0xfc, 0xa7, 0xfd, 0x33, 0xfe, 0xe5, 0xfe, 0x80, 0xff,
  0xb4, 0xfd, 0x71, 0xfd, 0x10, 0x00, 0x8f, 0xff, 0xb8, 0xfe, 0x72, 0xfe,
  0x9b, 0xfd, 0x65, 0xfd, 0xda, 0xfd, 0xc2, 0xfc, 0x7e, 0xfb, 0xcc, 0xfb,
  0x18, 0xfe, 0x42, 0xff, 0x83, 0xff, 0x91, 0xfe, 0xd3, 0xfd, 0x08, 0xff,
  0x26, 0xff, 0x76, 0xfe, 0x46, 0xfd, 0x3b, 0xfd, 0xbf, 0xfd, 0x01, 0xfd,
  0xc5, 0xfc, 0x5e, 0xfd, 0x0e, 0xfe, 0x56, 0xfd, 0x5b, 0xfc, 0x65, 0xfd,
  0x5e, 0xfe, 0x1d, 0xff, 0xbb, 0xfe, 0x3f, 0xfe, 0xe2, 0xfd, 0x0c, 0xfe,
  0xaf, 0xfe, 0x8b, 0xfd, 0x5d, 0xfe, 0x3b, 0x00, 0xd7, 0xff, 0x04, 0x00,
  0xf0, 0xff, 0x08, 0x00, 0xf4, 0xff, 0x02, 0x00, 0xf9, 0xff, 0xf7, 0xff,
  0xfe, 0xff, 0xfe, 0xff, 0x06, 0x00, 0x05, 0x00, 0xff, 0xff, 0xfa, 0xff,
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x04, 0x00,
  0x06, 0x00, 0x01, 0x00, 0xff, 0xff, 0x02, 0x00, 0x02, 0x00, 0xff, 0xff,
  0xff, 0xff, 0xfb, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0x02, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00
};
unsigned int tick_pcm_len = 1004;
