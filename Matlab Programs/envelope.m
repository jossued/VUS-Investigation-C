function [env, ht] = envelope(wf,varargin)
% Author: TJ Ulrich 5/23/05, updated 3/20/06 with code from RAG, see **
%   below.
% Purpose: create the envelope function of a given waveform, also provide
%   the hilbert transform of the original waveform
% Parameters: 
%   Input:
%       wf = waveform to be enveloped (preferably has a
%       length that is a power of 2)
%       sp = flag to indicate which method to use, i.e. is
%       the signal processing toolbox present.
%           0 = no (DO NOT use the signal proc. toolbox)
%           1 = yes (use Sig. Proc. toolbox)
%   Output: env = envelop to be returned
%           ht = Hilbert transform of wf
% 
% ** the code from Robert Guyer was used/altered to make this function not
% dependent upon the availability of the signal processing toolbox. The
% functionality using the sig. proc. toolbox is still available using a 1
% as the second input parameter. a 0 or no second parameter will neither
% require nor use the signal processing toolbox.
% 
% Syntax: 
%       env = envelope(wf) will produce the envelope of the waveform wf
%               without using the signal processing toolbox.
%       [env, HT] = envelope(wf) same as above but will also output the
%                   hilbert transform in the variable HT.
%       env = hilbert(wf,1) same as env = hilbert(wf) but WILL use the
%             signal processing toolbox (i.e., the hilbert() function)
%       [env, HT] = envelope(wf,1) same as above but will also output the
%               hilbert transform in the variable HT.
% check for aditional arguments, i.e. whether or not to use signal
% processing toolbox functions
if nargin > 1
    sp = varargin{1};
else
    sp = 0; % default value
end
% check for valid values of sp
switch sp
    case 0 % do nothing (valid value)
    case 1 % do nothing (valid value)
    otherwise
        sp = 0; % change to default valid value
end
% ----- create the envelope and Hilbert transform -----
if sp % use the signal processing toolbox
    env = abs(hilbert(wf));
    ht = imag(hilbert(wf));
else
    Nfft=length(wf);
    sz = size(wf);          % get size of wf ... i.e. row or column vector
    hlfsz1 = ceil(sz/2);    % get half the size (use ceil() for odd # of points)
    hlfsz2 = floor(sz/2);   % get half the size (use floor() for odd # of points)
    hlfsz1(hlfsz1<1) = 1;   % fix fractional dimensions
    hlfsz2(hlfsz2<1) = 1;   % fix fractional dimensions
    %%%%%% FFT
    FFTf=fft(wf,Nfft);
    %%%%%% Hilbert transform
    nplus=(1:ceil(Nfft/2));
    nminus=(1+ceil(Nfft/2):Nfft);
    Hfactor=zeros(sz);
    Hfactor(nplus)=ones(hlfsz1);
    Hfactor(nminus)=-ones(hlfsz2);
    %%%%%%
    Hf=Hfactor.*FFTf;
    %%%%%% in time domain
    f1=ifft(FFTf,Nfft);
    h1=ifft(Hf,Nfft);
    ht = imag(h1);
    %%%%%% calculate envelope
    e1=f1+h1;
    env=sqrt(e1.*conj(e1));
end