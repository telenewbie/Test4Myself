clear;
addpath('D:\code\AEC\subbandfilter\speech_kit');
addpath('D:\code\AEC\lib')
addpath('D:\code\beamforming\GSC\speech_kit\speech_kit')
addpath('D:\code\beamforming\GSC\lib')
% filePath = 'D:\wendang\项目\Cortana Validation\cortana test\cortana test 4-20200409\20200409_TEAMS\1\';
% [mixed_sig, fs] = audioread([filePath,'audio_all16k.wav']); %Far-end signal
% far = mixed_sig(:,1).*32768;
% near = mixed_sig(:,3).*32768;
% near = [zeros(0.02*fs,1) ;near(1:end-0.02*fs,:)];

% filePath='D:\wendang\项目\Cortana Validation\cortana test\cortana test 6-20200422\4\1\';
% fid=fopen([filePath,'far_in16k.pcm'], 'rb'); % Load far end
% far=fread(fid,inf,'float32');
% fclose(fid);
% fid=fopen([filePath,'near_in16k.pcm'], 'rb'); % Load near end
% near=fread(fid,inf,'float32');
% fclose(fid);
% far = far(1:length(near)).*32768;
% near = near.*32768;
% fs = 16000;
% filePath='D:\wendang\项目\Cortana Validation\cortana test\cortana test 6-20200422\1anechoic\2\';
% fid=fopen([filePath,'far_in.pcm'], 'rb'); % Load far end
% far=fread(fid,inf,'float32');
% fclose(fid);
% fid=fopen([filePath,'near_in.pcm'], 'rb'); % Load near end
% near=fread(fid,inf,'float32');
% fclose(fid);
% fs = 16000;
% far = resample(far,fs,48000);
% near = resample(near,fs,48000);
% far = far.*32768;
% near = near.*32768;
% near = [zeros(0.04*fs,1);near(1:end-0.04*fs,1)];

filePath='D:\code\AEC\download\echo-master\';
fid=fopen([filePath,'aecFar.pcm'], 'rb'); % Load far end
far=fread(fid,inf,'int16');
fclose(fid); 

fid=fopen([filePath,'aecNear.pcm'], 'rb'); % Load near end
near=fread(fid,inf,'int16');
fclose(fid);
fs = 16000;
len = length(far);
N = 64;
wlen = 2*N;
hop = wlen/2;
wins1 = ones(wlen,1);
%PBFDAF 重叠保留
xo = zeros(N,1);
do = xo;
eo = xo;
yo = xo;
Nframe = 1+fix((len-wlen)/N);
numfrq = N+1;
N = numfrq-1;
M = 12; %Number of partitions
alp = 0.9;% Power estimation factor alc = 0.1;
threshold=1.5e-5; %1.5e-6
%  用 1.5e-5可以消除高频回声，但是线性输出频谱泄露严重
%  1.5e-6   高频回声无法消除，但是线性输出无频谱泄露
% 大混响 需要调大 e-3
mufb = 0.6;
pn = 10*ones(numfrq,1);
XFm = zeros(numfrq,M);
YFb = zeros(numfrq,M);
WFb = zeros(numfrq,M);
wins1=[0;sqrt(hanning(2*N-1))];
WFbEn = zeros(M,Nframe);
zm = zeros(N,1);
linear_out = zeros(len,1);

%nlp
wins = [0;sqrt(hanning(2*N-1))];
%dtd
rem = zeros(numfrq,Nframe);
sigma_m2 = zeros(numfrq,Nframe);
alpha_dts = 0.6;
% echoBandRange = ceil(300*2/fs*(numfrq-1)):floor(4000*2/fs*(numfrq-1));
% mecc = 0;
% alpha_dt = 0.8;
%
variance_near = zeros(numfrq,Nframe);
variance_error = zeros(numfrq,Nframe);
gamma = zeros(numfrq,Nframe); % posteriori SNR
eta   = zeros(numfrq,Nframe); % priori SNR
gamma_max = 400;
eta_min=10^(-400/10);
alpha_dtd = 0.6*ones(numfrq,1);
alpha_s = 0; %  0 0.93
alpha = 0.85;
alpha_err = alpha_s;
alpha_res = alpha_s;

RES_ECHO = zeros(numfrq,Nframe);
ECHO = zeros(numfrq,Nframe);
variance_resecho = zeros(numfrq,Nframe);
gamma_n  = zeros(numfrq,Nframe);
eta_n    = zeros(numfrq,Nframe);
NLP_OUT = zeros(numfrq,Nframe);
NLP_OUT2 = zeros(numfrq,Nframe);
Gall = zeros(numfrq,Nframe);

% path change
last_max_w=0;
durition = -1;
max_psdw_diff = zeros(Nframe,1);
div_psdw = zeros(Nframe,1);

NLP1 = 1;
NLP2 = 1;
xfwm=zeros(numfrq,M);
echoBandRange = ceil(300*2/fs*N):floor(1800*2/fs*N);
cohxd = zeros(numfrq,1);
Se = zeros(numfrq,1);
Sd = zeros(numfrq,1);
Sx = zeros(numfrq,1);
Sed = zeros(numfrq,1);
Sxd = zeros(numfrq,1);
alpha_gamma = 0.93;
hnledp=[];
hnled = zeros(numfrq, 1);
weight=zeros(numfrq,1);
hnlMax = zeros(numfrq, 1);
hnl = zeros(numfrq, 1);
overdrive = ones(1, numfrq);

fbSupp = 0;
hnlLocalMin = 1;
cohxdLocalMin = 1;

dkEnV=zeros(Nframe,1);
ekEnV=zeros(Nframe,1);
ovrd = 2;
ovrdPos = floor((N+1)/4);
ovrdSm = 2;
hnlMin = 1;
minCtr = 0;
SeMin = 0;
SdMin = 0;
SeLocalAvg = 0;
SeMinSm = 0;
divergeFact = 1;
dIdx = 1;
hnlMinCtr = 0;
hnlNewMin = 0;
divergeState = 0;
mult=fs/8000;
cohxdMax = 0;
cohed_all = zeros(numfrq,Nframe);
od = ones(numfrq,1);
Se0 = zeros(numfrq,1);
Sd0 = zeros(numfrq,1);
Sed0 = zeros(numfrq,1);
%% process
for n=1:Nframe
    pos = N * (n-1) + 1;
    xk = far(pos:pos+N-1);
    dk = near(pos:pos+N-1);
    xx = [xo;xk];
    xo = xk;
    tmp11 = fft(xx);
    XX = tmp11(1:numfrq);
    dd = [do;dk];
    do = dk;
    tmp = fft(dd);
    DD = tmp(1:numfrq);
    
    %%
    % adaptive filter
    pn = alp*pn + (1-alp)*M*real(XX.*conj(XX));
    XFm(:,1) = XX;
    for m=1:M
        YFb(:,m) = XFm(:,m).*WFb(:,m);
    end
    yfk = sum(YFb,2);
    tmp = [yfk; flipud(conj(yfk(2:end-1)))];
    ykt = real(ifft(tmp));
    ykfb = ykt(end-N+1:end); % adaptive filter echo estimate
    
    ekfb = dk - ykfb;  % output
    linear_out(pos:pos+N-1) = ykfb;
    
    tmp = fft([zm;ekfb]);
    Ek = tmp(1:N+1);
    Ek2 = Ek ./(pn + 1e-10); % Normalized error
    absEf = abs(Ek2);
    
    for frq=1:N+1
        if absEf(frq) > threshold
            absEf(frq) = threshold/(absEf(frq)+1e-10);
            Ek2(frq) = Ek2(frq)*absEf(frq);
        end
    end
    
    mEk = mufb.*Ek2;
    PP = conj(XFm).*(ones(M,1) * mEk')';
    tmp = [PP ; flipud(conj(PP(2:N,:)))];
    IFPP = real(ifft(tmp));
    PH = IFPP(1:N,:);
    tmp = fft([PH;zeros(N,M)]);
    FPH = tmp(1:N+1,:);
    WFb = WFb + FPH;
    XFm(:,2:end) = XFm(:,1:end-1);
    WFbEn(:,n) = sum(real(WFb.*conj(WFb)));
    [max_w,idx_w] = max(WFbEn(:,n));
    %     max_psdw_diff(n) = abs(last_max_w - max_w);
    %     if n>4
    %         div_psdw(n) = sum(max_psdw_diff(n-4:n));
    %     end
    %     if div_psdw(n)>8 && durition <=0
    %         durition = 75;
    %         %         WFb = zeros(numfrq,M,numRef);
    %         n
    %         linear_out(pos:pos+N-1) = 10000000;
    %     else
    %         durition = durition-1;
    %     end
    %     %     if durition >0
    %     %         mufb = 1;
    %     %         threshold= 1.5e-6*ones(numfrq,1); %1.5e-6
    %     %     else
    %     %         mufb = 1;
    %     %         threshold= 1.5e-7*ones(numfrq,1); %1.5e-6
    %     %     end
    %     last_max_w = max_w;
    %     [tmp,index] = max(WFbEn);
    
    %%
    %nlp
    ee = [eo;ekfb];
    eo = ekfb;
    tmp = fft(ee.*wins);
    error_spec = tmp(1:numfrq);
    tmp = fft(dd.*wins);
    near_spec = tmp(1:numfrq);
    tmp = fft(xx.*wins);
    far_spec = tmp(1:numfrq);
    
    yy=[yo;ykfb];
    yo = ykfb;
    tmp = fft(yy.*wins);
    echoEst_spec = tmp(1:numfrq);
    echoEst_spec1(:,n) = echoEst_spec;
    echoEst_spec2(:,n) = near_spec-error_spec;
    if NLP1
        alpha_s = real(alpha_dtd);
        alpha_err = real(alpha_dtd);
        alpha_res = real(alpha_dtd);
        %     alpha_dts = real(alpha_dtd);
        if n==1
            variance_near(:,n)  = (1-alpha_s).*real(near_spec.*conj(near_spec));
            variance_error(:,n) = (1-alpha_err).*real(error_spec.*conj(error_spec));
        else
            variance_near(:,n)  = alpha_s.*variance_near(:,n-1)+(1-alpha_s).*abs(near_spec).^2;
            variance_error(:,n) = alpha_err.*variance_error(:,n-1)+(1-alpha_err).*abs(error_spec).^2;
        end
        %         gamma(:,n) = abs(near_spec(:,n)).^2 ./(abs(error_spec(:,n)).^2+eps);% posteriori SNR
        gamma(:,n) = variance_near(:,n) ./(variance_error(:,n)+eps);
        %         gamma(:,n) = min(gamma(:,n),gamma_max);
        
        if n==1
            eta(:,n) =  alpha + (1- alpha)*max(gamma(:,n)-1,0);
        else
            eta(:,n) = alpha*eta_2term + (1-alpha)*max(gamma(:,n)-1,0);
        end
        eta(:,n) = max(eta_min,eta(:,n));
        v = gamma(:,n).*eta(:,n)./(1+eta(:,n));
        
        GH1=ones(numfrq,1);
        idx=find(v>5);
        GH1(idx)=eta(idx,n)./(1+eta(idx,n));
        idx=find(v<=5 & v>0);
        GH1(idx)=eta(idx,n)./(1+eta(idx,n)).*exp(0.5*expint(v(idx)));
        %         GH1 = eta(:,n)./(1+eta(:,n)).*exp(0.5*expint(v));
        eta_2term=GH1.^2.*gamma(:,n);
        ECHO(:,n) = GH1.*near_spec;
        %         echoEst_spec = near_spec - error_spec;
        RES_ECHO(:,n) = GH1.*near_spec-echoEst_spec;
        %%%%%%%%%LSA for estimate near end signal e = v+b %%%%%%%%%%%%
        if n==1
            variance_resecho(:,n) = (1-alpha_res).*abs(RES_ECHO(:,n)).^2;
        else
            variance_resecho(:,n) = alpha_res.*variance_resecho(:,n-1)+(1-alpha_res).*abs(RES_ECHO(:,n)).^2;
        end
        %         gamma_n(:,n) = abs(error_spec(:,n)).^2 ./ (abs(RES_ECHO(:,n)).^2 + eps);
        gamma_n(:,n) = variance_error(:,n) ./ (variance_resecho(:,n) + eps);
        %         gamma_n(:,n) = min(gamma_n(:,n),gamma_max);
        if n==1
            eta_n(:,n) =  alpha + (1- alpha)*max(gamma_n(:,n)-1,0);
        else
            eta_n(:,n) =  alpha*eta_n2term + (1-alpha)*max(gamma_n(:,n)-1,0);
        end
        eta_n(:,n) = max(eta_min,eta_n(:,n));
        vn = gamma_n(:,n).*eta_n(:,n)./(1+eta_n(:,n));
        GH1_n=ones(numfrq,1);
        idx=find(vn>5);
        GH1_n(idx)=eta_n(idx,n)./(1+eta_n(idx,n));
        idx=find(vn<=5 & vn>0);
        GH1_n(idx)=eta_n(idx,n)./(1+eta_n(idx,n)).*exp(0.5*expint(vn(idx)));
        %         GH1 = eta(:,n)./(1+eta(:,n)).*exp(0.5*expint(v));
        eta_n2term=GH1_n.^2.*gamma_n(:,n);
        Gall(:,n) = GH1_n;
        %     mag = GH1_n .* abs(error_spec(:,n));
        %     ang = GH1_n .* angle(error_spec(:,n)) + (1-GH1_n) .* rand(numfrq,1)*2*pi;
        %     NLP_OUT(:,n) = mag.*exp(1j*ang);
        NLP_OUT(:,n) = GH1_n.*error_spec;
        %         H = min(1, sqrt(Tv./abs(RES_ECHO(:,n)).^2));
        %         near_est(:,n) = H.*error_spec(:,n);
        %%
        %dtd
        if n==1
            rem(:,n) = (1-alpha_dts).*error_spec.*conj(near_spec);
            sigma_m2(:,n) = (1-alpha_dts).*near_spec.*conj(near_spec);
        else
            rem(:,n) = alpha_dts.*rem(:,n-1)+(1-alpha_dts).*error_spec.*conj(near_spec);
            sigma_m2(:,n) = alpha_dts.*sigma_m2(:,n-1)+(1-alpha_dts).*near_spec.*conj(near_spec);
        end
        alpha_dtd = real(1-abs(rem(:,n))./sigma_m2(:,n));
        alpha_dtd_all(:,n) = alpha_dtd;
        alpha_dtd = max(alpha_dtd,0);
        alpha_dtd = min(alpha_dtd,1);
    end
    ef0 = NLP_OUT(:,n);
    df0 = near_spec;
    Se0 = alpha_gamma*Se0 + (1-alpha_gamma)*real(ef0.*conj(ef0));
    Sd0 = alpha_gamma*Sd0 + (1-alpha_gamma)*real(df0.*conj(df0));
    Sed0 = alpha_gamma*Sed0 + (1-alpha_gamma)*ef0.*conj(df0);
    cohed0 = real(Sed0.*conj(Sed0))./(Se0.*Sd0 + 1e-10);
    cohed0 = min(1,cohed0);
    GH1_n = GH1_n.*cohed0;
%     NLP_OUT(:,n) = NLP_OUT(:,n).*cohed0;
    if NLP2
        ef = error_spec;
        df = near_spec;
        xfwm(:,1) = far_spec;
        xf = xfwm(:,idx_w);
%         alpha_gamma = 1-alpha_dtd;
        Se = alpha_gamma.*Se + (1-alpha_gamma).*real(ef.*conj(ef));
        Sd = alpha_gamma.*Sd + (1-alpha_gamma).*real(df.*conj(df));
        Sx = alpha_gamma.*Sx + (1 - alpha_gamma).*real(xf.*conj(xf));
        Sxd = alpha_gamma.*Sxd + (1 - alpha_gamma).*xf.*conj(df);
        Sed = alpha_gamma.*Sed + (1-alpha_gamma).*ef.*conj(df);
        cohed = real(Sed.*conj(Sed))./(Se.*Sd + 1e-10);
                cohedall(:,n) = cohed;
        cohxd = real(Sxd.*conj(Sxd))./(Sx.*Sd + 1e-10);
        %         cohxdall(:,n) = cohxd;
        hnled = min(1 - cohxd, cohed);
        cohedMean = mean(cohed(echoBandRange));
        %         cohedMeanall(n)=cohedMean;
        [hnlSort, hnlSortIdx] = sort(1-cohxd(echoBandRange));
        [xSort, xSortIdx] = sort(Sx);
        hnlSortQ = mean(1 - cohxd(echoBandRange));
        [hnlSort2, hnlSortIdx2] = sort(hnled(echoBandRange));
        hnlQuant = 0.75;
        hnlQuantLow = 0.5;
        qIdx = floor(hnlQuant*length(hnlSort2));
        qIdxLow = floor(hnlQuantLow*length(hnlSort2));
        hnlPrefAvg = hnlSort2(qIdx);
        hnlPrefAvgLow = hnlSort2(qIdxLow);
        if cohedMean > 0.98 && hnlSortQ > 0.9
            suppState = 0;
        elseif cohedMean < 0.95 || hnlSortQ < 0.8
            suppState = 1;
        end
        %     suppState = 0;
        if hnlSortQ < cohxdLocalMin && hnlSortQ < 0.75
            cohxdLocalMin = hnlSortQ;
        end
        
        if cohxdLocalMin == 1
            ovrd = 3;
            hnled = 1-cohxd;
            hnlPrefAvg = hnlSortQ;
            hnlPrefAvgLow = hnlSortQ;
        end
        if suppState == 0
            hnled = cohed;
            hnlPrefAvg = cohedMean;
            hnlPrefAvgLow = cohedMean;
        end
        % Track the local filter minimum to determine suppression overdrive.
        if hnlPrefAvgLow < hnlLocalMin & hnlPrefAvgLow < 0.6
            hnlLocalMin = hnlPrefAvgLow;
            hnlMin = hnlPrefAvgLow;
            hnlNewMin = 1;
            hnlMinCtr = 0;
        end
        if hnlNewMin == 1
            hnlMinCtr = hnlMinCtr + 1;
        end
        if hnlMinCtr == 2  % <0.6  >0.6
            hnlNewMin = 0;
            hnlMinCtr = 0;
            ovrd = max(log(0.00001)/(log(hnlMin + 1e-10) + 1e-10), 1);
%             ovrd = max(hnlMin/(log(hnlMin + 1e-10) + 1e-10), 1);
        end
        hnlLocalMin = min(hnlLocalMin + 0.0008/mult, 1);
        cohxdLocalMin = min(cohxdLocalMin + 0.0004/mult, 1);
        if ovrd < ovrdSm
            ovrdSm = 0.99*ovrdSm + 0.01*ovrd;
        else
            ovrdSm = 0.9*ovrdSm + 0.1*ovrd;
        end
        aggrFact = 0.3;
        wCurve = [0; aggrFact*sqrt(linspace(0,1,numfrq-1))' + 0.1];
        weight = wCurve;
                hnled = weight.*min(hnlPrefAvg,hnled) + (1 - weight).*hnled;
%         hnled = min(hnlPrefAvg,min(hnled,GH1_n));
        hnl_all(:,n) = hnled;
        od = ovrdSm*(sqrt(linspace(0,1,numfrq))' + 1);
        od_all(:,n) = od;
        sshift = ones(numfrq,1);
        hnled = hnled.^(od.*sshift);
%         hnl = hnled.^3;
        ef = ef.*(hnled);
        NLP_OUT2(:,n) = ef;
        xfwm(:,2:end) = xfwm(:,1:end-1);
    end
   
    
end
% near_spec_all  = stft_aec(near,wins1);
% figure;
% subplot(211);imagesc(log10(abs(near_spec_all)));axis([0 Nframe 0 numfrq]);
% a = alpha_dtd_all;
% a = max(a,0);
% a = min(a,1);
% subplot(212);imagesc(a)
idx ='test';
% audiowrite([filePath,'linear_out_160-4_',idx,'.wav'],linear_out./32768,fs);
output = istft_aec(NLP_OUT2,wins,len);
audiowrite([filePath,'output_160-4_160_nlp2_',idx,'.wav'],output./32768,fs);
