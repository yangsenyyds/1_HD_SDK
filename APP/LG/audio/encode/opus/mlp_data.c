#include "opus_config.h"
#ifdef HAVE_CONFIG_H

#endif

#include "mlp.h"

/* RMS error was 0.280492, seed was 1480478173 */
/* 0.005976 0.031821 (0.280494 0.280492) done */

static const float weights[450] = {

/* hidden layer */
-0.514624f, 0.0234227f, -0.14329f, -0.0878216f, -0.00187827f,
-0.0257443f, 0.108524f, 0.00333881f, 0.00585017f, -0.0246132f,
0.142723f, -0.00436494f, 0.0101354f, -0.11124f, -0.0809367f,
-0.0750772f, 0.0295524f, 0.00823944f, 0.150392f, 0.0320876f,
-0.0710564f, -1.43818f, 0.652076f, 0.0650744f, -1.54821f,
0.168949f, -1.92724f, 0.0517976f, -0.0670737f, -0.0690121f,
0.00247528f, -0.0522024f, 0.0631368f, 0.0532776f, 0.047751f,
-0.011715f, 0.142374f, -0.0290885f, -0.279263f, -0.433499f,
-0.0795174f, -0.380458f, -0.051263f, 0.218537f, -0.322478f,
1.06667f, -0.104607f, -4.70108f, 0.312037f, 0.277397f,
-2.71859f, 1.70037f, -0.141845f, 0.0115618f, 0.0629883f,
0.0403871f, 0.0139428f, -0.00430733f, -0.0429038f, -0.0590318f,
-0.0501526f, -0.0284802f, -0.0415686f, -0.0438999f, 0.0822666f,
0.197194f, 0.0363275f, -0.0584307f, 0.0752364f, -0.0799796f,
-0.146275f, 0.161661f, -0.184585f, 0.145568f, 0.442823f,
1.61221f, 1.11162f, 2.62177f, -2.482f, -0.112599f,
-0.110366f, -0.140794f, -0.181694f, 0.0648674f, 0.0842248f,
0.0933993f, 0.150122f, 0.129171f, 0.176848f, 0.141758f,
-0.271822f, 0.235113f, 0.0668579f, -0.433957f, 0.113633f,
-0.169348f, -1.40091f, 0.62861f, -0.134236f, 0.402173f,
1.86373f, 1.53998f, -4.32084f, 0.735343f, 0.800214f,
-0.00968415f, 0.0425904f, 0.0196811f, -0.018426f, -0.000343953f,
-0.00416389f, 0.00111558f, 0.0173069f, -0.00998596f, -0.025898f,
0.00123764f, -0.00520373f, -0.0565033f, 0.0637394f, 0.0051213f,
0.0221361f, 0.00819962f, -0.0467061f, -0.0548258f, -0.00314063f,
-1.18332f, 1.88091f, -0.41148f, -2.95727f, -0.521449f,
-0.271641f, 0.124946f, -0.0532936f, 0.101515f, 0.000208564f,
-0.0488748f, 0.0642388f, -0.0383848f, 0.0135046f, -0.0413592f,
-0.0326402f, -0.0137421f, -0.0225219f, -0.0917294f, -0.277759f,
-0.185418f, 0.0471128f, -0.125879f, 0.262467f, -0.212794f,
-0.112931f, -1.99885f, -0.404787f, 0.224402f, 0.637962f,
-0.27808f, -0.0723953f, -0.0537655f, -0.0336359f, -0.0906601f,
-0.0641309f, -0.0713542f, 0.0524317f, 0.00608819f, 0.0754101f,
-0.0488401f, -0.00671865f, 0.0418239f, 0.0536284f, -0.132639f,
0.0267648f, -0.248432f, -0.0104153f, 0.035544f, -0.212753f,
-0.302895f, -0.0357854f, 0.376838f, 0.597025f, -0.664647f,
0.268422f, -0.376772f, -1.05472f, 0.0144178f, 0.179122f,
0.0360155f, 0.220262f, -0.0056381f, 0.0317197f, 0.0621066f,
-0.00779298f, 0.00789378f, 0.00350605f, 0.0104809f, 0.0362871f,
-0.157708f, -0.0659779f, -0.0926278f, 0.00770791f, 0.0631621f,
0.0817343f, -0.424295f, -0.0437727f, -0.24251f, 0.711217f,
-0.736455f, -2.194f, -0.107612f, -0.175156f, -0.0366573f,
-0.0123156f, -0.0628516f, -0.0218977f, -0.00693699f, 0.00695185f,
0.00507362f, 0.00359334f, 0.0052661f, 0.035561f, 0.0382701f,
0.0342179f, -0.00790271f, -0.0170925f, 0.047029f, 0.0197362f,
-0.0153435f, 0.0644152f, -0.36862f, -0.0674876f, -2.82672f,
1.34122f, -0.0788029f, -3.47792f, 0.507246f, -0.816378f,
-0.0142383f, -0.127349f, -0.106926f, -0.0359524f, 0.105045f,
0.291554f, 0.195413f, 0.0866214f, -0.066577f, -0.102188f,
0.0979466f, -0.12982f, 0.400181f, -0.409336f, -0.0593326f,
-0.0656203f, -0.204474f, 0.179802f, 0.000509084f, 0.0995954f,
-2.377f, -0.686359f, 0.934861f, 1.10261f, 1.3901f,
-4.33616f, -0.00264017f, 0.00713045f, 0.106264f, 0.143726f,
-0.0685305f, -0.054656f, -0.0176725f, -0.0772669f, -0.0264526f,
-0.0103824f, -0.0269872f, -0.00687f, 0.225804f, 0.407751f,
-0.0612611f, -0.0576863f, -0.180131f, -0.222772f, -0.461742f,
0.335236f, 1.03399f, 4.24112f, -0.345796f, -0.594549f,
-76.1407f, -0.265276f, 0.0507719f, 0.0643044f, 0.0384832f,
0.0424459f, -0.0387817f, -0.0235996f, -0.0740556f, -0.0270029f,
0.00882177f, -0.0552371f, -0.00485851f, 0.314295f, 0.360431f,
-0.0787085f, 0.110355f, -0.415958f, -0.385088f, -0.272224f,
-1.55108f, -0.141848f, 0.448877f, -0.563447f, -2.31403f,
-0.120077f, -1.49918f, -0.817726f, -0.0495854f, -0.0230782f,
-0.0224014f, 0.117076f, 0.0393216f, 0.051997f, 0.0330763f,
-0.110796f, 0.0211117f, -0.0197258f, 0.0187461f, 0.0125183f,
0.14876f, 0.0920565f, -0.342475f, 0.135272f, -0.168155f,
-0.033423f, -0.0604611f, -0.128835f, 0.664947f, -0.144997f,
2.27649f, 1.28663f, 0.841217f, -2.42807f, 0.0230471f,
0.226709f, -0.0374803f, 0.155436f, 0.0400342f, -0.184686f,
0.128488f, -0.0939518f, -0.0578559f, 0.0265967f, -0.0999322f,
-0.0322768f, -0.322994f, -0.189371f, -0.738069f, -0.0754914f,
0.214717f, -0.093728f, -0.695741f, 0.0899298f, -2.06188f,
-0.273719f, -0.896977f, 0.130553f, 0.134638f, 1.29355f,
0.00520749f, -0.0324224f, 0.00530451f, 0.0192385f, 0.00328708f,
0.0250838f, 0.0053365f, -0.0177321f, 0.00618789f, 0.00525364f,
0.00104596f, -0.0360459f, 0.0402403f, -0.0406351f, 0.0136883f,
0.0880722f, -0.0197449f, 0.089938f, 0.0100456f, -0.0475638f,
-0.73267f, 0.037433f, -0.146551f, -0.230221f, -3.06489f,
-1.40194f, 0.0198483f, 0.0397953f, -0.0190239f, 0.0470715f,
-0.131363f, -0.191721f, -0.0176224f, -0.0480352f, -0.221799f,
-0.26794f, -0.0292615f, 0.0612127f, -0.129877f, 0.00628332f,
-0.085918f, 0.0175379f, 0.0541011f, -0.0810874f, -0.380809f,
-0.222056f, -0.508859f, -0.473369f, 0.484958f, -2.28411f,
0.0139516f,
/* output layer */
3.90017f, 1.71789f, -1.43372f, -2.70839f, 1.77107f,
5.48006f, 1.44661f, 2.01134f, -1.88383f, -3.64958f,
-1.26351f, 0.779421f, 2.11357f, 3.10409f, 1.68846f,
-4.46197f, -1.61455f, 3.59832f, 2.43531f, -1.26458f,
0.417941f, 1.47437f, 2.16635f, -1.909f, -0.828869f,
1.38805f, -2.67975f, -0.110044f, 1.95596f, 0.697931f,
-0.313226f, -0.889315f, 0.283236f, 0.946102f, };

static const int topo[3] = {25, 16, 2};

const MLP net = {
    3,
    topo,
    weights
};
