# toy_AES
An implementation of AES(Advance Encryption Standard)，used for learning!

# How to run 

make &&  ./aes_test 
  
You will see:

```
  key:
  00 01 02 03 
  04 05 06 07 
  08 09 0a 0b 
  0c 0d 0e 0f 

  userdata:
  00 11 22 33 
  44 55 66 77 
  88 99 aa bb 
  cc dd ee ff 

  encrypt:
  69 c4 e0 d8 
  6a 7b 04 30 
  d8 cd b7 80 
  70 b4 c5 5a 

  decrypt:
  00 11 22 33 
  44 55 66 77 
  88 99 aa bb 
  cc dd ee ff
```


# Advanced Encryption Standard (Rijndael) 实现原理

## 一、介绍

Advanced Encryption Standard (AES)，或者别名 Rijndael，是美国 NIST 于2001年公布的一个密码加密算法。AES算法是 Rijndael 算法的一个子集。AES 声明了加密块大小 16 Byte，密钥长度支持 128、192、256三种bit长度。

在AES标准确定前，在全世界范围内，进行了5年的算法征集、评选工作。最终 Rijndael算法被选中。

声明，本文所有内容都来自于文末参考的论文、wikepedia内容学习总结而来。记录下来用于总结备记。水平有限，可能存在理解错误！

## 二、Mathematical preliminaries

本节引入部分代数运算。AES里面的运算环节用到了 polynomial 在GF(2^8)有限域下的加法、乘法运算。这里的加法、乘法，和我们在实数域的加法（+），乘法（*）有些差异，因此这里引入前置的数学定义、运算。

### Polynomial Addition in GF(2^8)

GF(2^8)为Golois finite 有限域，容纳0~255个值。

这里描述多项式的加法运算。这里的一个多项式代表一个字节（8 bit)，所以一个字节可以表示为一个多项式的形式。多项式的系数只能是0或1，指数是0~7。

AES首先定义了通过一个简单多项式表示一个Byte。

例子：

如 {0100，0011}：

	X^6 + X + 1 	(polynomial notation)

又如 {0000,1001},表示为：

	X^3 + 1 		(polynomial notation)

那么，AES定义这里多项式加法定义和我们平常说的 XOR（异或）一致。


	(x^6 + x^4 + x^2 + x +1) + (x^7 + x +1) = x^7 + x^6 + x^4 + x^2 	(polynomial notation)
	{01010111} ¯ {10000011} = {11010100} 								(binary notation)
	{57} ¯ {83} = {d4} 													(hexadecimal notation)


### Polynomial Multiplication in GF(2^8)

AES还定义了一个在GF(2^8)内执行多项式的乘法运算，也就是在GF(2^8)内的单字节之间的乘法运算。

AES定义的单字节乘法 \* 乘法运算步骤为：

step 1,先执行一次实数域内的多项式乘法运算。

step 2,然后执行多项式下取模 m(x)
其中 m(x) = x^8 + x^4 + x^3 + x +1

参考图示：

### Polynomia Multiplication by x in GF(2^8)

前一节已经描述了多项式的乘法运算，这里描述的多项式 * X只是一个特例而已。

这个特例可以通过迭代多次运算快速获得结果。

图：左乘 x 

### Polynomials with Coefficients in GF(2^8)

前面3节都只是限定表达的值是1个Byte。假设我们需要表达一个4byte（Four-term polynomials）的值怎么办？AES给出另外一种表达方式，这个方式也是用polynomial表达，但是这个多项式和前面的不一样，前面的多项式系数只有0或1,并且X^i代表第i位的bit为1。而下面的方式，每个系数本身表示的就是一个byte，系数取值范围为[0,255]，如下：

	a(x) = a3 x^3 + a2x^2 + a1x + a0

上面代表的是4个Byte的值 【a0,a1,a2,a3】。

AES定义4个Byte下的加法 + 运算格式为：

	if 
	a(x) =  a3 x^3 + a2x^2 + a1x + a0,
	b(x) =  b3 x^3 + b2x^2 + b1x + b0

	a(x)+b(x) = (a3 xor b3)x^3 + (a2 xor b2)x^2 + (a1 oxr b1)x + (a0 xor b0)

总结下就是指数执行异或操作（xor)，所以还是收敛在4-term 的polynomial里面的。

AES定义的4个byte下的乘法 * 运算格式为：

step1,先执行polynomial下的乘法展开，涉及到 单字节乘法 * 的按polynomial 下的单字节乘法预算。

step2，对结果取模 （x^4 + 1)。由于取模后，指数会<4，因此还是一个4-term bytes的值。

图：4-term multiplication


此外，上面的4-term 的polynomial 运算是按特定格式的。这个多项式乘法 a(x) * b(x) 可以通过下面的矩阵左乘b(x)简化运算（注意，这里矩阵的单字节乘法还是AES下的polynomial单字节乘法）。

	a0 a3 a2 a1 
	a1 a0 a3 a2	
	a2 a1 a0 a3
	a3 a2 a1 a0 

图：矩阵转换



#### 逆元

前面定义了4-term 的polynomial 乘法。介绍这么多AES的运算定义，AES只是为了做1件事情，为AES加密流程构建特别的映射，确保这个映射是1对1的，这种映射是为了混淆并达成某些目的（抵抗密码学上的attacks）。

对于一个4-term 的polynomial 的乘法预算，容易知道 1 即为其单位元。同时根据定义，单字节的多项式加法满足交换律、结合律，乘法满足交换律、结合律、分配率，4-term的同理。 对于4-term 的polynomial，如果能找到 a'(x) * a(x) = 1,那么对于b(x),通过 a(x) * b(x) =c(x)可以做一次映射，并且 a'(x) * c(x) = b(x)。定义a'(x)是a(x)的逆元。

AES定义了a(x) 和 a'(x)，

	a(x) = {03}x^3 + {01}x^2 + {01}x + {02} 
	a'(x) = {0b}x^3 + {0d}x^2 + {09}x + {0e}

AES将在 mixColumns 步骤将会通过a(x)进行映射、打乱4 byte。解密的时候，通过a'(x)转换回来。



## 三、Algorithm Specification 

本节定义AES 算法的定义。 

AES算法是 block cipher 类型，本文讲解的(Rijndael）实现也是围绕 block 进行。论文里面AES定义的 block 大小是固定为 **4*4** 字节的方块。在论文里面用 **state** 来表示。

AES算法有几个关键的参数，Nb：表示block有多少个word（AES里面就只能是4），Nk：表示密钥的word长度，取值可以分别是4/6/8，每个word为4字节，Nr：代表每个block进行多少轮次的加密，这个是算法针对不同密钥长度有不同的定义。

图：Nr 和Nk 的关系


### 
### Cipher 运算过程

下面是算法实际执行加密的流程，每个块 block 会进行 **AddRoundKey** 、**SubBytes** 、 **MixColumns**、**ShiftRow**这几项运算。
这几个操作后文会介绍。
	

	Note: Specification copy from paper[1]

	Cipher(byte in[4*Nb], byte out[4*Nb], word w[Nb*(Nr+1)])
	begin
		byte state[4,Nb]
		state = in

		AddRoundKey(state, w[0, Nb-1]) // See Sec. 5.1.4

		for round = 1 step 1 to Nr–1
			SubBytes(state) // See Sec. 5.1.1
			ShiftRows(state) // See Sec. 5.1.2
			MixColumns(state) // See Sec. 5.1.3
			AddRoundKey(state, w[round*Nb, (round+1)*Nb-1])
		end for

		SubBytes(state)
		ShiftRows(state)
		AddRoundKey(state, w[Nr*Nb, (Nr+1)*Nb-1])

		out = state
	end

### SubBytes过程
S-box是单字节的打乱，直接针对 block 的每个字节进行替换所得。这里可通过 256 Byte的数组映射即可。

图：s-box 替换



图：s-box mapping

### ShiftRows过程

shift rows的过程比较直观，把4*4的block按row进行移位。每列移位数在论文里面有公式计算。简言之：

	第一列，不用shift
	第二列，shift = 3
	第三列，shift = 2
	第四列，shift = 1

图：shift rows 



### MixColumns过程

这里的运算就是前面讲的4-term 针对block的每列，左乘 a(x) = {03}x^3 + {01}x^2 + {01}x + {02} 即可。

这个左乘a(x)等同于以下矩阵左乘block的矩阵运算，但有一点不同，矩阵里面元素的乘法是 polynomial 在GF(2^8)下面的乘法。

	
	2 3 1 1
	1 2 3 1
	1 1 2 3
	3 1 1 2

图：mix columns 



### AddRoundKey过程

这里直接通过拿轮 i 的密钥和block进行 xor 运算。 一个Nk*4 byte的密钥会根据Nk的大小,Nk可取4、6、8，扩展为Nr+1个轮密钥，每个轮密钥的长度为 4*4。注意，我们传入的密钥，AES会进行密钥扩展，参考下节。

图：AES定义轮Nr个数和Nk的关系

图：

图：xor运算


### 密钥扩展

AES密钥扩展会根据用户密钥作为种子，然后每4字节进行扩展。每次扩展的4字节会考虑 （第前1个4字节）和（第前Nk个4字节），进行一定的调整顺序、异或运算得来。


	Note：copy from Dijndael paper[1]

	KeyExpansion(byte key[4*Nk], word w[Nb*(Nr+1)], Nk)
	begin
		word temp
		i = 0
		while (i < Nk)
			w[i] = word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3])
			i = i+1
		end while
		i = Nk
		while (i < Nb * (Nr+1)]
			temp = w[i-1]
			if (i mod Nk = 0)
				temp = SubWord(RotWord(temp)) xor Rcon[i/Nk]
			else if (Nk > 6 and i mod Nk = 4)
				temp = SubWord(temp)
			end if
			w[i] = w[i-Nk] xor temp
			i = i + 1
		end while
	end

图：key expansion


### Inverse Cipher 运算过程

这部分的运算过程非常明确，针对每一步，AES算法定义都有对应的可逆实现。比如S-box里面是256个byte取值的一对一映射，这个步骤逆转只需要构造逆向的S-inverse-box即可。其它几个步骤同理可得。此处略去。


## 四、 算法设计灵感

在AES算法作者提交的proposal里面，特地有一个章节讲述了AES算法设计的motivation。里面描述了算法设计过程参考了大量的密码学上常见的攻击，在算法设计过程中，每一步都是为了规避这些已知攻击办法，并且在每一步的时候设置严格的criteria来指导设计算法。

这里详细请参考论文《The Rijndael Block Cipher》里面的 Motivation for design choices，可以知道作者背后的设计灵感来源。


## 五、 算法实现

密码学涉及较多数学知识、论证，而且算法是标准的。因此，工程上采用开源的事实标准最好。AES算法有非常多不同语言、平台的实现，而且这些实现往往和其它协议（如TLS）、加密算法一起提供。

参考下列wikipedia，罗列各类语言、平台的开源AES实现。

https://en.wikipedia.org/wiki/AES_implementations

### 事实标准：openssl

从事实上，openssl应该是在后端服务（特别是linux类）平台，有广泛的使用。在C/C++类平台，应该用 **openssl** 开源库。

### AES算法加速

从AES specification上我们可以知道，通过预先运算，可以在矩阵乘法里面省略大量的运算过程，如 4-term polynomial里面的运算，构建256*256的表可以快速的完成运算。进一步的，shift rows、s-box也可以表示为一个矩阵过程,通过矩阵预先运算，可省去大量的实时计算过程。

另一个方面，在cpu指令集上，部分cpu厂商的指令集内置了AES的相关操作指令，如Intel 、AMD。

详细参考 wikipedia 
https://en.wikipedia.org/wiki/Advanced_Encryption_Standard#Performance


### 玩具AES实现

为了学习并确认我们对AES的理解，可按论文自行实现。本文给出一个样例，该样例是作者参考论文描述做的直观实现，没有对实现做任何预计算优化。




## 六、 加密的分组模式

AES明确确定了一个block大小为 4*4 Bytes，而上面描述都是针对这个block如何加密做的描述。

在实际用户加密时，我们加密前需要选择一个合适分组模式。分组模式 block cipher算法都有的。

常见分组模式包括：ECB（不推荐使用）、CBC、PCBC、CFB、OFB、CTR。

这些分组模式部分需要 IV（初始化向量），部分不需要（如ECB）。

当我们用一些算法的时候需要指明用什么模式，比如在 openssl 中，有如 "EVP_aes_128_ecb" ，代表选择AES,密钥128bit，分组模式CBC。

相对而言，分组模式推荐用CTR模式，比如AES-GCM就是用CTR进行分组加密，通过GMAC做消息验证码。具体原因参考分组模式之间的优劣。

本文集中在AES算法，分组模式不展开介绍。参考wikipedia。

https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Common_modes



## 参考

1.Advanced Encryption Standard wikipedia

https://en.wikipedia.org/wiki/Advanced_Encryption_Standard


2.Joan Daemen and Vincent Rijmen (September 3, 1999). "AES Proposal: Rijndael" (PDF). Archived from the original (PDF) on February 3, 2007.

https://web.archive.org/web/20070203204845/https://csrc.nist.gov/CryptoToolkit/aes/rijndael/Rijndael.pdf

3.Announcing the ADVANCED ENCRYPTION STANDARD (AES) 

https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.197.pdf

4.Block_cipher_mode_of_operation

https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Common_modes


