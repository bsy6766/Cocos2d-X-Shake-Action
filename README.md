# CCShakeAction

## What is this?
A shaking action for Cocos2d-x. It shakes the cocos2d node.

## How and what to use
I recommend you to use Simplex Noise version than Perlin Noise.<br>
Clone the repo and copy source code to your project.<br>

```cpp
auto sprite = cocos2d::Sprite::create("HelloWorld.png");
sprite->runAction(ActionShake::create(2.0f/*Duration*/, 40.0f/*speed*/, 40.0f/*magnitude*/));
```

## Preview
![Preview](https://github.com/bsy6766/CCShakeAction/blob/master/preview.gif)<br>
i.e.) Duration = 2.0f, Magnitude = 40.0f, Speed = 40.0f

## Reference
[Perlin Noise](https://en.wikipedia.org/wiki/Perlin_noise).<br>
[Simplex Noise](https://en.wikipedia.org/wiki/Simplex_noise) and [Reference](http://webstaff.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf).

## Note
- Uses Perlin Noise and Simplex Noise<br>
- Perlin Noise shake tested on Cocos2d-x 3.10 version.
- Simplex Noise shake tested on Cocos2d-x 3.14.1 version.
- Can be used like other built-in actions. Read comment in code for details.
- I did not write the core algorithm for Perlin Noise, but made it to work on Cocos2d-x 3.10 version because none of them out on the internet worked as described.
- I followed the pdf reference linked above to implement Simplex Noise.
