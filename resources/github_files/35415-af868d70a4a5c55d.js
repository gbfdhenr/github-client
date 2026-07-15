performance.mark("js-parse-end:35415-af868d70a4a5c55d.js");
"use strict";(globalThis.rspackChunk_github_ui_github_ui=globalThis.rspackChunk_github_ui_github_ui||[]).push([[35415],{110731(e,t,r){e.exports=r.p+"security-be7ea1e143b16773.png"},164636(e,t,r){let n="GraphQLTraces",i="GraphQLTracingRefresh",s=a()?decodeURIComponent(new URLSearchParams(window.location.search).get("disable_clusters")||"").split(",").filter(e=>""!==e):[];function a(){return"u">typeof window}function o(e){if(!a()||!l()||!e)return;let t=window;t&&!t[n]&&(t[n]=[]),t&&e.__trace&&(t[n].push(e.__trace),"function"==typeof t[i]&&t[i]())}function l(){if(!a())return!1;let e=window;return"true"===new URLSearchParams(window.location.search).get("_tracing")||e&&void 0!==e[n]}function c(){return s.length>0}function u(e){if(!a()||!l()&&!c())return e;let t=new URL(e,window.location.origin);return l()&&t.searchParams.set("_tracing","true"),c()&&t.searchParams.set("disable_clusters",s.join(",")),t.pathname+t.search}function d(e){return s.indexOf(e)>-1}function f(e){if(!a())return;let t=s.indexOf(e);t>-1?s.splice(t,1):s.push(e);let r=new URLSearchParams(window.location.search);r.set("disable_clusters",s.join(",")),window.location.search=r.toString()}r.d(t,{Av:()=>o,HX:()=>d,M_:()=>f,RD:()=>u},{BM:n,rb:i})},291726(e,t,r){r.d(t,{Pt:()=>j,VT:()=>Y.A,$M:()=>d,Ay:()=>f,zh:()=>v,Rp:()=>m.A,rU:()=>h.A,Tz:()=>n,pt:()=>g.p,PF:()=>g.P,mU:()=>u});var n=r(110731),i=r(474848),s=r(916522),a=r(296540),o=r(276413);let l=(0,a.createContext)(null);l.displayName="WebGLContext";let c=(0,a.createContext)(null);c.displayName="WebGLArtworkContext";let u=()=>{let e,t=(0,s.c)(3),r=(0,a.useContext)(l),n=(0,a.useContext)(c);return t[0]!==n||t[1]!==r?(e={...r,artwork:n},t[0]=n,t[1]=r,t[2]=e):e=t[2],e};function d(e){let t,r,n,l,c=(0,s.c)(11),{className:d,params:f}=e,{registerBrandedDivider:v,unregisterBrandedDivider:h}=u(),g=(0,a.useRef)(null),m=(0,a.useId)(),[p,x]=(0,a.useState)("1600 / 157");return c[0]!==m||c[1]!==f||c[2]!==v||c[3]!==h?(t=()=>{g.current&&v({id:m,element:g.current,params:f});let e=()=>{window.innerWidth<=o.S.MOBILE?x("390 / 115"):window.innerWidth<=o.S.TABLET?x("800 / 131"):x("1600 / 157")};return e(),window.addEventListener("resize",e),()=>{h(m),window.removeEventListener("resize",e)}},c[0]=m,c[1]=f,c[2]=v,c[3]=h,c[4]=t):t=c[4],c[5]===Symbol.for("react.memo_cache_sentinel")?(r=[],c[5]=r):r=c[5],(0,a.useEffect)(t,r),c[6]!==p?(n={width:"100%",maxWidth:"1600px",aspectRatio:p},c[6]=p,c[7]=n):n=c[7],c[8]!==d||c[9]!==n?(l=(0,i.jsx)("div",{ref:g,className:d,style:n}),c[8]=d,c[9]=n,c[10]=l):l=c[10],l}function f(e){let t,r,n,o,l,c,d=(0,s.c)(18),{className:f,params:v}=e,h=(0,a.useRef)(null),g=(0,a.useRef)(!1),m=(0,a.useRef)(0),{registerDither:p,unregisterDither:x}=u(),y=(0,a.useId)();d[0]!==y?(t=()=>`${y}-${m.current}`,d[0]=y,d[1]=t):t=d[1];let w=t;return d[2]!==w||d[3]!==v||d[4]!==p||d[5]!==x?(r=()=>{h.current&&(g.current&&(x(w()),g.current=!1),m.current=m.current+1,p({id:w(),element:h.current,params:v}),g.current=!0)},d[2]=w,d[3]=v,d[4]=p,d[5]=x,d[6]=r):r=d[6],d[7]!==v.color||d[8]!==v.dotOpacity||d[9]!==v.fadeIn||d[10]!==v.type?(n=[v.color,v.type,v.fadeIn,v.dotOpacity],d[7]=v.color,d[8]=v.dotOpacity,d[9]=v.fadeIn,d[10]=v.type,d[11]=n):n=d[11],(0,a.useEffect)(r,n),d[12]!==w||d[13]!==x?(o=()=>()=>{g.current&&(x(w()),g.current=!1)},d[12]=w,d[13]=x,d[14]=o):o=d[14],d[15]===Symbol.for("react.memo_cache_sentinel")?(l=[],d[15]=l):l=d[15],(0,a.useEffect)(o,l),d[16]!==f?(c=(0,i.jsx)("div",{ref:h,className:f}),d[16]=f,d[17]=c):c=d[17],c}function v({children:e,mode:t="light"}){let[n,s]=(0,a.useState)([]),[o,u]=(0,a.useState)([]),d=(0,a.useRef)({dithers:[],brandedDividers:[],enterpriseHero:null}),f=(0,a.useRef)({artwork:null,listeners:new Set}),h=(0,a.useCallback)(e=>(f.current.listeners.add(e),()=>f.current.listeners.delete(e)),[]),g=(0,a.useSyncExternalStore)(h,()=>f.current.artwork,()=>null),m=(0,a.useCallback)(e=>{s(t=>t.find(t=>t.id===e.id)?t:[...t,e])},[]),p=(0,a.useCallback)(e=>{s(t=>t.filter(t=>t.id!==e))},[]),x=(0,a.useCallback)(e=>{u(t=>t.some(t=>t.id===e.id)?t:[...t,e])},[]),y=(0,a.useCallback)(e=>{u(t=>t.filter(t=>t.id!==e))},[]),w=(0,a.useCallback)(e=>{d.current.enterpriseHero||(d.current.enterpriseHero=e,k.current?.updateRegisterEnterpriseHero(e))},[]),b=(0,a.useCallback)(e=>{d.current.enterpriseHero?.id===e&&(d.current.enterpriseHero=null,k.current?.unregisterEnterpriseHero())},[]),P=(0,a.useRef)(null),k=(0,a.useRef)(null),I=(0,a.useRef)(null);(0,a.useEffect)(()=>{if(P.current){let e=!1,n=e=>{for(let t of(f.current.artwork=e,f.current.listeners))t()},i=()=>{if(k.current&&P.current){let e=document.body.clientWidth,t=window.innerHeight;k.current.resize(e,t)}},s=()=>{k.current?.update(),I.current=requestAnimationFrame(s)};return(async()=>{let{default:a}=await r.e(7427).then(r.bind(r,867632));if(e)return;let o=new a({canvas:P.current});k.current=o,n(o),"light"===t?o.switchBackgroundColor({isWhite:!0}):o.switchBackgroundColor({isWhite:!1}),o.updateRegisterDither(d.current.dithers),o.updateRegisterBrandedDivider(d.current.brandedDividers),d.current.enterpriseHero&&o.updateRegisterEnterpriseHero(d.current.enterpriseHero),s(),i(),(async()=>{await document.fonts?.ready,i()})()})(),window.addEventListener("resize",i),()=>{e=!0,k.current=null,n(null),window.removeEventListener("resize",i),I.current&&cancelAnimationFrame(I.current)}}},[]),(0,a.useEffect)(()=>{d.current.dithers=n,k.current&&k.current.updateRegisterDither(n)},[n]),(0,a.useEffect)(()=>{d.current.brandedDividers=o,k.current&&k.current.updateRegisterBrandedDivider(o)},[o]);let z=(0,a.useMemo)(()=>({registerDither:m,unregisterDither:p,registerBrandedDivider:x,unregisterBrandedDivider:y,registerEnterpriseHero:w,unregisterEnterpriseHero:b}),[m,p,x,y,w,b]);return(0,i.jsx)(l,{value:z,children:(0,i.jsx)(c,{value:g,children:(0,i.jsxs)("div",{style:{position:"relative",overflow:"hidden"},children:[(0,i.jsx)("canvas",{ref:P,style:{pointerEvents:"none",position:"absolute",left:0,zIndex:0},"aria-hidden":"true",tabIndex:-1}),e]})})})}d.displayName="WebGLBrandedDivider",f.displayName="WebGLDither",v.displayName="WebGLPage";var h=r(579885),g=r(393237),m=r(780069),p=r(710961),x=r(439437),y=r(617888);let w=r.p+"copilot_head-e5051da1a2302051.glb",b=r.p+"ear-e66a523a033aab1e.webp",P=r.p+"eyes-d65c2b4389677cd6.webp",k=r.p+"glass-d4c3f0a6ef341a78.webp",I=r.p+"goggle-9a6fe9e86c3010f5.webp",z=r.p+"head-9f9717fa05af369d.webp",C=r.p+"matcap-e3d5ea725297708f.webp",S=r.p+"screen-d774d3666b0ea059.webp",M=r.p+"vento-cf31c4a05898b3c7.webp",_={Ears:{src:b,texture:null,flipY:!1,isFace:!0,isGoggle:!1,fresnelIntensity:.6,fresnelColor:new x.Q1f(0xa9fbff),fresnelPosRange:new x.I9Y(2,-1),matcapIntensity:1,specularIntensity:1},Eyes:{src:P,texture:null,flipY:!1,isFace:!0,isGoggle:!1,fresnelIntensity:0,fresnelColor:new x.Q1f(0xbf70ff),fresnelPosRange:new x.I9Y(2,-1),matcapIntensity:0,specularIntensity:0},Glass:{src:k,texture:null,flipY:!1,isFace:!1,isGoggle:!0,fresnelIntensity:1,fresnelColor:new x.Q1f(5452467),fresnelPosRange:new x.I9Y(0,1),matcapIntensity:.5,specularIntensity:0},Goggle:{src:I,texture:null,flipY:!1,isFace:!1,isGoggle:!0,fresnelIntensity:.5,fresnelColor:new x.Q1f(9559807),fresnelPosRange:new x.I9Y(0,1),matcapIntensity:2,specularIntensity:0},Head:{src:z,texture:null,flipY:!1,isFace:!0,isGoggle:!1,fresnelIntensity:.6,fresnelColor:new x.Q1f(0xa9fbff),fresnelPosRange:new x.I9Y(2,-2),matcapIntensity:1,specularIntensity:1},Screen:{src:S,texture:null,flipY:!1,isFace:!0,isGoggle:!1,fresnelIntensity:1,fresnelColor:new x.Q1f(1118102),fresnelPosRange:new x.I9Y(2,-1),matcapIntensity:0,specularIntensity:1},Vents:{src:M,texture:null,flipY:!1,isFace:!0,isGoggle:!1,fresnelIntensity:0,fresnelColor:new x.Q1f(0xbf70ff),fresnelPosRange:new x.I9Y(2,-1),matcapIntensity:1,specularIntensity:1}},E={matcap:{src:C,texture:null,flipY:!0}},F={head:{src:w,scene:new x.Z58}};class G{originalPos;mousemoveFuncs;pos;targetElementPos;targetElementSize;isMousemove=!1;constructor(e,t){this.targetElementPos=e,this.targetElementSize=t,this.originalPos=new x.I9Y,this.mousemoveFuncs=[],this.pos={target:new x.I9Y(0,0),current:new x.I9Y(0,0),current2:new x.I9Y(0,0)}}onMousemove(e){this.isMousemove=!0;let t=(e.clientX-this.targetElementPos.x)/this.targetElementSize.x;t=(t-.5)*2;let r=(e.clientY-this.targetElementPos.y)/this.targetElementSize.y;r=(.5-r)*2,this.updateMousePos(t,r)}onTouchstart(e){if(this.isMousemove=!0,e.touches[0]){let t=(e.touches[0].clientX-this.targetElementPos.x)/this.targetElementSize.x;t=(t-.5)*2;let r=(e.touches[0].clientY-this.targetElementPos.y)/this.targetElementSize.y;r=(.5-r)*2,this.updateMousePos(t,r)}}updateMousePos(e,t){for(let r of(this.pos.target.set(e,t),this.mousemoveFuncs))r()}addMousemoveFunc(e){this.mousemoveFuncs.push(e)}resize(){}update(e){this.pos.current.lerp(this.pos.target,Math.min(1,2*e)),this.pos.current2.lerp(this.pos.target,Math.min(1,1.5*e))}}let N=`

uniform sampler2D map;
uniform sampler2D matcap;
uniform float fresnelBias;
uniform float fresnelScale;
uniform float fresnelPower;
uniform float uFresnelIntensity;
uniform vec3 uFresnelColor;
uniform vec2 uFresnelPosRange;
uniform float uSpecularIntensity;
uniform vec3 uLightPosition;

varying vec3 vViewPosition;
varying vec2 vUv;
varying vec3 vNormal;
varying vec3 vLocalPosition;

//http://gamedev.stackexchange.com/questions/59797/glsl-shader-change-hue-saturation-brightness
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


void main() {
        vec4 diffuseColor = vec4(1.0);
        vec4 sampledDiffuseColor = texture2D( map, vUv );
        diffuseColor *= sampledDiffuseColor;

        vec3 normal = normalize( vNormal );
        float light = dot(normal, normalize(uLightPosition)) * 0.5 + 0.5;
        float specular_1 = dot(normal, normalize(vec3(0.5, 1, 1))) * 0.5 + 0.5;
        specular_1 = smoothstep(0.95, 1.0, specular_1);

        float specular_2 = dot(normal, normalize(vec3(-0.5, -1, 1))) * 0.5 + 0.5;
        specular_2 = smoothstep(0.95, 1.0, specular_1);

        float specular = max(specular_1, specular_2) * uSpecularIntensity;




        vec3 viewDir = normalize( vViewPosition );
        vec3 x = normalize( vec3( viewDir.z, 0.0, - viewDir.x ) );
        vec3 y = cross( viewDir, x );
        vec2 uv = vec2( dot( x, normal ), dot( y, normal ) ) * 0.495 + 0.5;

        vec4 matcapColor = texture2D( matcap, uv );
        matcapColor = linearToOutputTexel(matcapColor);

        vec3 outgoingLight = diffuseColor.rgb;

        float fp = fresnelPower;

        float fresnel = fresnelBias + fresnelScale * pow(1.0 - dot(normalize(vNormal), vec3(0.0, 0.0, 1.0)), fp);

        fresnel = min(fresnel, 1.0);


        float gradientPos = smoothstep(uFresnelPosRange.x, uFresnelPosRange.y, vLocalPosition.y); // Gradient based on y-position
        vec4 fresnelColor = vec4(uFresnelColor, 1.0);

        gl_FragColor = vec4( outgoingLight, diffuseColor.a );
        gl_FragColor = linearToOutputTexel( gl_FragColor );
        gl_FragColor = mix(gl_FragColor, fresnelColor, uFresnelIntensity * fresnel * gradientPos);


        float glowIntensity = length(vec3(0.0, -1.0, 0.0) - vLocalPosition);
        glowIntensity = smoothstep(0.5, 0.0, glowIntensity);

        vec3 hsv = rgb2hsv(gl_FragColor.rgb);
        hsv.r -= glowIntensity * 0.05;
        hsv.g += 0.1 - specular * 0.2;
        hsv.b += mix(-0.2, 0.1, matcapColor.g) + specular * 0.1;
        hsv.b = clamp(hsv.b, 0.0, 1.0);
        gl_FragColor.rgb = hsv2rgb(hsv);
        gl_FragColor.rgb += glowIntensity * 0.2 + light * 0.3 * uFresnelIntensity;



        gl_FragColor.rgb = pow(gl_FragColor.rgb, vec3(1.8));

        // debug
        // gl_FragColor.rgb = vec3(fresnel);
}
`,R=`

uniform bool uIsEye;
uniform float uWink;
varying vec2 vUv;
varying vec3 vNormal;
varying vec3 vViewPosition;
varying vec3 vLocalPosition;

float parabola(float x){
        return -pow(2.0 * x, 2.0);
}



void main() {
        vUv = uv;
        vec3 objectNormal = normal;
        vec3 transformedNormal = objectNormal;
        transformedNormal = normalMatrix * transformedNormal;
        vNormal = normalize( transformedNormal );

        vec3 transformed = vec3( position );
        vLocalPosition = transformed;

        vec4 worldPosition = modelMatrix * vec4(transformed, 1.0);
        vViewPosition.y = worldPosition.y;

        transformed.y += parabola(transformed.x) * uWink * 2.0;

        vec4 mvPosition = vec4( transformed, 1.0 );
        mvPosition = modelViewMatrix * mvPosition;
        vViewPosition = - mvPosition.xyz;
        gl_Position = projectionMatrix * mvPosition;
}
`,T=new x.Pq0(.6,.6,1);class j{group=new x.YJl;animationGroup_1=new x.YJl;animationGroup_2=new x.YJl;animationGroup_3=new x.YJl;goggleGroup=new x.YJl;faceGroup=new x.YJl;eyesMeshes={left:null,right:null};targetElement;targetElementSize=new x.I9Y(1,1);targetElementPos=new x.I9Y(0,0);windowSize;meshes=[];uniforms={fresnelBias:{value:0},fresnelScale:{value:2},fresnelPower:{value:1},uLightPosition:{value:new x.Pq0(.5,1,-1).normalize()}};isInitTlPlayed=!1;initTl;mouseMng;lookatProgress={target:0,current:0};lookatTarget=new x.Pq0(0,0,1);lookatTarget_default=new x.Pq0(0,0,1);globalPosition=new x.Pq0(0,0,0);isBlinking=!1;blinkTimer=null;constructor(e,t){this.targetElement=e,this.windowSize=t,this.group.add(this.animationGroup_1),this.animationGroup_1.add(this.animationGroup_2),this.animationGroup_2.add(this.animationGroup_3),this.animationGroup_3.add(this.goggleGroup),this.animationGroup_3.add(this.faceGroup),this.mouseMng=new G(this.targetElementPos,this.targetElementSize),this.initTl=p.Ay.timeline({paused:!0,onComplete:()=>{this.createBlinkEyes()}}),(e=>{let t,r,n;Promise.all([...(t=new x.Tap,Object.values(E).map(e=>new Promise((r,n)=>{t.load(e.src,t=>{e.texture=t,t.flipY=e.flipY,r(t)},void 0,e=>n(e instanceof Error?e:Error("Asset load failed")))}))),...(r=new y.B,Object.values(F).map(e=>new Promise((t,n)=>{r.load(e.src,r=>{e.scene=r.scene,t(r.scene)},void 0,e=>n(e instanceof Error?e:Error("Asset load failed")))}))),...(n=new x.Tap,Object.values(_).map(e=>new Promise((t,r)=>{n.load(e.src,r=>{e.texture=r,r.flipY=e.flipY,t(r)},void 0,e=>r(e instanceof Error?e:Error("Asset load failed")))})))]).then(()=>{e&&e()})})(()=>{this.init(),this.initTl.add(()=>{this.group.visible=!0}).fromTo(this.animationGroup_1.position,{y:-2},{y:0,ease:"elastic.out(1,0.5)",duration:1.5},0).fromTo(this.animationGroup_3.scale,{x:0,y:0,z:0},{x:1,y:1,z:1,ease:"expo.out",duration:.3},0)})}init(){for(let[e,t]of Object.entries(_))if(t.texture){let r=F.head.scene?.getObjectByName(e);if(r)switch(e){case"Eyes":case"Ears":case"Goggle":case"Glass":{let n=this.createMaterial(t,e),i=this.createMaterial(t,e),s=new x.eaF(r.geometry.clone(),n),a=new x.eaF(r.geometry.clone(),i);s.name=`${e}_left`,a.name=`${e}_right`,a.geometry.scale(-1,1,1),"Eyes"===e&&(s.position.copy(r.position),a.position.copy(r.position),a.position.x*=-1,this.eyesMeshes.left=s,this.eyesMeshes.right=a),this.meshes.push({mesh:s,isGoggle:t.isGoggle,isFace:t.isFace}),this.meshes.push({mesh:a,isGoggle:t.isGoggle,isFace:t.isFace}),t.isGoggle&&(this.goggleGroup.add(s),this.goggleGroup.add(a)),t.isFace&&(this.faceGroup.add(s),this.faceGroup.add(a));break}default:{let n=this.createMaterial(t,e),i=new x.eaF(r.geometry.clone(),n);i.name=e,this.meshes.push({mesh:i,isGoggle:t.isGoggle,isFace:t.isFace}),i.matrixWorldNeedsUpdate=!0,t.isGoggle&&this.goggleGroup.add(i),t.isFace&&this.faceGroup.add(i)}}}}createMaterial(e,t){let r={uWink:{value:0}},n={...this.uniforms,map:{value:e.texture},matcap:{value:E.matcap.texture},uFresnelIntensity:{value:e.fresnelIntensity},uIsEye:{value:"Eyes"===t},uFresnelColor:{value:e.fresnelColor},uFresnelPosRange:{value:e.fresnelPosRange},uSpecularIntensity:{value:e.specularIntensity},...r},i=new x.BKk({vertexShader:R,fragmentShader:N,uniforms:n,transparent:!0,side:2,defines:{GLASS:+("Glass"===t)}});return i.userData.uniforms=r,i}createBlinkEyes(){this.isBlinking=!0;let e=p.Ay.timeline({onComplete:()=>{this.isBlinking=!1;let e=1e3*x.cj9.lerp(2,6,Math.random());this.blinkTimer=setTimeout(()=>{this.createBlinkEyes()},e)},defaults:{ease:"power3.out"}}),t=.8>Math.random()?"blinkType1":"blinkType2",r=[this.eyesMeshes.left?this.eyesMeshes.left.scale:new x.Pq0(1,1,1),this.eyesMeshes.right?this.eyesMeshes.right.scale:new x.Pq0(1,1,1)];"blinkType1"===t?e.to(r,.1,{y:.1},0).to(r,.3,{y:1},.2):e.to(r,.1,{y:.1},0).to(r,.2,{y:1},.2).to(r,.3,{y:.1},.4).to(r,.3,{y:1},.7)}resize(){}onMousemove(e){this.mouseMng.onMousemove(e)}onTouchstart(e){this.mouseMng.onTouchstart(e)}update(e,t){let r=this.targetElement.getBoundingClientRect(),n=r.left+r.width/2-this.windowSize.x/2,i=-(r.top+r.height/2-this.windowSize.y/2);this.group.position.set(n,i,-(2*(.25*r.width))),this.targetElementPos.set(r.left,r.top),this.targetElementSize.set(r.width,r.height);let s=r.bottom>=0&&r.right>=0&&r.top<=this.windowSize.y&&r.left<=this.windowSize.x;!this.isInitTlPlayed&&s&&(this.isInitTlPlayed=!0,this.initTl.play()),this.group.scale.set(.25*r.width,.25*r.width,.25*r.width);let a=this.mouseMng.pos.target.length();this.lookatProgress.target=x.cj9.clamp(x.cj9.mapLinear(a,5,2,0,1),0,1),this.mouseMng.isMousemove||(this.lookatProgress.target=0),this.lookatProgress.current+=(this.lookatProgress.target-this.lookatProgress.current)*Math.min(1,3*t),this.lookatTarget_default.copy(T),this.lookatTarget.set(.2*this.mouseMng.pos.current.x,.2*Math.max(this.mouseMng.pos.current.y,0),1),this.lookatTarget.lerp(this.lookatTarget_default,1-this.lookatProgress.current);let o=new x.Pq0;this.animationGroup_2.getWorldPosition(o),this.lookatTarget.x+=o.x,this.lookatTarget.y+=o.y,this.lookatTarget.z+=o.z,this.animationGroup_2.lookAt(this.lookatTarget),this.mouseMng.update(t)}}var Y=r(668394)},276413(e,t,r){r.d(t,{},{S:{TABLET:1011,MOBILE:767}})},579885(e,t,r){var n=r(620778),i=r(160641),s=r(839189);let a=`${s.A+i.A}
uniform vec4 uProgress;
uniform vec3 uColor1;
uniform vec4 uRectData;
uniform vec3 uBgColor;

varying vec3 vPosition;
varying vec3 vNormal;

void main(){
  vec3 no = abs(vNormal);
  float maxComponent = max(no.x, max(no.y, no.z));
  no = vec3(equal(no, vec3(maxComponent)));

  vec2 rainbowFactor = vec2(
    snoise(vec3(vPosition * 0.25 + no * 0.2 + uProgress.y + 2.0)),
    snoise(vec3(vPosition * 0.5 + no + uProgress.y * 2.0))
  ) * 0.5 + 0.5;


  vec3 rainbowColor = uColor1;
  vec3 hsv = rgb2hsv(rainbowColor);
  hsv.r -= rainbowFactor.x * 0.2;
  hsv.g -= rainbowFactor.y * 0.1;
  hsv.b += rainbowFactor.y * 0.2;

  #ifndef IS_GLASS_RENDER
  ${n.A}
  #endif

  rainbowColor = hsv2rgb(hsv);
  rainbowColor += mix(0.0, 0.05, uBgColor.r);
  gl_FragColor = vec4(rainbowColor, 1.0);
}
`;r.d(t,{},{A:a})},393237(e,t,r){var n=r(620778);let i=`
  varying vec2 vUv;
  void main(){
    vUv = uv;
    gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
  }
`,s=`
  varying vec2 vUv;
  uniform sampler2D uTexture;
  uniform vec4 uRectData;
  uniform vec4 uProgress;

  void main(){
    vec4 color = texture2D(uTexture, vUv);
    ${n.A}

    color.rgb /= color.a;
    gl_FragColor = color;
    gl_FragColor.a *= uProgress.x;
  }
`;r.d(t,{},{P:i,p:s})},620778(e,t,r){let n=`
if(gl_FragCoord.y < uRectData.w - uRectData.y || 
    gl_FragCoord.y > uRectData.w || 
    gl_FragCoord.x < uRectData.z || 
    gl_FragCoord.x > uRectData.z + uRectData.x){
    discard;
}
`;r.d(t,{},{A:n})},160641(e,t,r){let n=`
vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
    
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

`;r.d(t,{},{A:n})},839189(e,t,r){let n=`
float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

// simplex noise
// TODO Optimize it more
vec4 permute(vec4 x){return mod(x*x*34.0+x,289.);}
vec4 taylorInvSqrt(vec4 r){ return 1.79284291400159 - 0.85373472095314 * r; }

float snoise(vec3 v){
  const vec2  C = vec2(0.166666667, 0.33333333333) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 = v - i + dot(i, C.xxx) ;
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy;
  vec3 x3 = x0 - D.yyy;
  i = mod(i,289.);
  vec4 p = permute( permute( permute(
	  i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
	+ i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
	+ i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
  vec3 ns = 0.142857142857 * D.wyz - D.xzx;
  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);
  vec4 x_ = floor(j * ns.z);
  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = floor(j - 7.0 * x_ ) *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);
  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));
  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m * m;
  return 42.0 * dot( m, vec4( dot(p0,x0), dot(p1,x1),dot(p2,x2), dot(p3,x3) ) );
}
`;r.d(t,{},{A:n})},780069(e,t,r){let n=`
varying vec2 vUv;
varying vec3 vNormal;
varying vec3 vNormal2;

varying vec3 vPosition;
varying vec3 vWorldPosition;

#ifdef MOSAIC 
    varying vec2 vScreenPos;
    varying vec2 vCenterPos;
#endif

void main(){
    vUv = uv;
    vNormal = normal;
    vNormal2 = normalize(normalMatrix * normal);
    vPosition = position;

    vec4 worldPos = modelMatrix * vec4(position, 1.0);
    vec4 screenPos = projectionMatrix * viewMatrix * worldPos;
    vWorldPosition = worldPos.xyz;

    #ifdef MOSAIC 
        vec4 centerpos = projectionMatrix * modelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0);
        vCenterPos = centerpos.xy / centerpos.w;
        vScreenPos = screenPos.xy / screenPos.w;
    #endif

    gl_Position = screenPos;
}
`;r.d(t,{},{A:n})},668394(e,t,r){var n=r(439437),i=r(751566);class s extends n.LoY{constructor(e=.1,t=4){super();const r=new n.bdM(1,1);r.translate(0,0,.5+e);const s=r.clone(),a=r.clone();a.rotateY(Math.PI/2);const o=r.clone();o.rotateY(Math.PI);const l=r.clone();l.rotateY(-Math.PI/2);const c=r.clone();c.rotateX(-Math.PI/2);const u=r.clone();u.rotateX(Math.PI/2);const d=new n.Gu$(e,t,t,0,.5*Math.PI,0,.5*Math.PI);d.translate(-.5,.5,.5);const f=d.clone(),v=d.clone();v.rotateZ(Math.PI/2);const h=d.clone();h.rotateZ(Math.PI);const g=d.clone();g.rotateZ(-Math.PI/2);const m=f.clone();m.rotateY(Math.PI);const p=v.clone();p.rotateY(Math.PI);const x=h.clone();x.rotateY(Math.PI);const y=g.clone();y.rotateY(Math.PI);const w=new n.Ho_(e,e,1,t,1,!0,0,Math.PI/2);w.translate(.5,0,.5);const b=w.clone(),P=w.clone();P.rotateZ(Math.PI/2);const k=w.clone();k.rotateZ(Math.PI);const I=w.clone();I.rotateZ(-Math.PI/2);const z=b.clone();z.rotateY(Math.PI);const C=P.clone();C.rotateY(Math.PI);const S=k.clone();S.rotateY(Math.PI);const M=I.clone();M.rotateY(Math.PI);const _=w.clone();_.rotateX(Math.PI/2);const E=_.clone();E.rotateZ(Math.PI/2);const F=_.clone();F.rotateZ(Math.PI);const G=_.clone();G.rotateZ(-Math.PI/2);const N=(0,i.h0)([s,a,o,l,c,u,f,v,h,g,m,p,x,y,b,P,k,I,z,C,S,M,_,E,F,G]);this.copy(N)}}r.d(t,{A:()=>s})},904369(e,t,r){var n=r(296540);let i={type:"browser",isBrowser:!0,isServer:!1},s={type:"server",isBrowser:!1,isServer:!0};function a(){return()=>{}}function o(){return i}function l(){return s}function c(){return(0,n.useSyncExternalStore)(a,o,l)}r.d(t,{V:()=>c})},840090(e,t,r){r.d(t,{W:()=>v});var n=r(474848),i=r(887993),s=r(916522),a=r(355368),o=r(296540),l=r(432314);function c(e){return e.replace(/\/$/,"")||"/"}let u=e=>{if(void 0===e.actual)return!1;let t=new URL(e.url,void 0),r=new URL(e.actual,void 0),n=t.hostname===r.hostname,i=c(t.pathname)===c(r.pathname);return n&&i};var d=r(798669);let f="subnav",v=(0,i.tx)()?e=>{let t,r,i,c,v,h,g,m,p,x,y,w,b,P,k=(0,s.c)(42);k[0]!==e?({ref:i,component:t,...r}=e,k[0]=e,k[1]=t,k[2]=r,k[3]=i):(t=k[1],r=k[2],i=k[3]);let[I,z]=(0,o.useState)(void 0);k[4]!==I||k[5]!==t.fields.subheading?(c=t.fields.subheading&&!u({url:t.fields.subheading.fields.href,actual:I}),k[4]=I,k[5]=t.fields.subheading,k[6]=c):c=k[6];let C=c;k[7]===Symbol.for("react.memo_cache_sentinel")?(v=()=>{z(window.location.href)},h=[],k[7]=v,k[8]=h):(v=k[7],h=k[8]),(0,o.useEffect)(v,h);let S=r["data-color-mode"],M=r.className,_=r.hasShadow,E=`subnav-heading-link-${t.fields.heading.sys.id}`,F=t.fields.heading.fields.href;if(k[9]!==t.fields.heading.fields.text?(g=(0,d.$C)({action:t.fields.heading.fields.text,tag:"link",location:f}),k[9]=t.fields.heading.fields.text,k[10]=g):g=k[10],k[11]!==t.fields.heading.fields.href||k[12]!==t.fields.heading.fields.text||k[13]!==E||k[14]!==g?(m=(0,n.jsx)(a.SubNav.Heading,{"data-ref":E,href:F,...g,children:t.fields.heading.fields.text}),k[11]=t.fields.heading.fields.href,k[12]=t.fields.heading.fields.text,k[13]=E,k[14]=g,k[15]=m):m=k[15],k[16]!==I||k[17]!==t.fields.subheading?(p=t.fields.subheading&&(0,n.jsx)(a.SubNav.SubHeading,{"data-ref":`subnav-subheading-link-${t.fields.subheading.sys.id}`,href:t.fields.subheading.fields.href,...u({url:t.fields.subheading.fields.href,actual:I})?{"aria-current":"page"}:{},...(0,d.$C)({action:t.fields.subheading.fields.text,tag:"link",location:f}),children:t.fields.subheading.fields.text}),k[16]=I,k[17]=t.fields.subheading,k[18]=p):p=k[18],k[19]!==I||k[20]!==t.fields.links||k[21]!==r.linkVariant){let e;k[23]!==I||k[24]!==r.linkVariant?(e=e=>{if((0,l.P)(e)){let{href:t,text:i}=e.fields,s=u({url:t,actual:I});return(0,n.jsx)(a.SubNav.Link,{"data-ref":`subnav-link-${e.sys.id}`,href:t,variant:r.linkVariant,...s?{"aria-current":"page"}:{},...(0,d.$C)({action:i,tag:"link",location:f}),children:i},e.sys.id)}let{heading:t,links:i}=e.fields,s=void 0===i?[]:i;return(0,n.jsxs)(a.SubNav.Link,{href:t.fields.href,variant:r.linkVariant,...u({url:t.fields.href,actual:I})?{"aria-current":"page"}:{},children:[t.fields.text,s.length>0&&(0,n.jsx)(a.SubNav.SubMenu,{children:s.map(e=>(0,n.jsx)(a.SubNav.Link,{href:e.fields.href,variant:r.linkVariant,"data-ref":`subnav-submenu-link-${e.sys.id}`,...u({url:e.fields.href,actual:I})?{"aria-current":"page"}:{},...(0,d.$C)({action:e.fields.text,tag:"link",context:t.fields.text,location:f}),children:e.fields.text},e.sys.id))})]},t.sys.id)},k[23]=I,k[24]=r.linkVariant,k[25]=e):e=k[25],x=t.fields.links.map(e),k[19]=I,k[20]=t.fields.links,k[21]=r.linkVariant,k[22]=x}else x=k[22];return k[26]!==t.fields.cta?(y=void 0!==t.fields.cta&&(0,n.jsx)(a.SubNav.Action,{href:t.fields.cta.fields.href,"data-ref":`subnav-cta-action-${t.fields.cta.sys.id}`,children:t.fields.cta.fields.text}),k[26]=t.fields.cta,k[27]=y):y=k[27],k[28]!==r.className||k[29]!==r.hasShadow||k[30]!==i||k[31]!==m||k[32]!==p||k[33]!==x||k[34]!==y?(w=(0,n.jsxs)(a.SubNav,{className:M,hasShadow:_,ref:i,children:[m,p,x,y]}),k[28]=r.className,k[29]=r.hasShadow,k[30]=i,k[31]=m,k[32]=p,k[33]=x,k[34]=y,k[35]=w):w=k[35],k[36]!==C?(b=C&&(0,n.jsx)(a.Box,{paddingBlockStart:{narrow:64,regular:"none"}}),k[36]=C,k[37]=b):b=k[37],k[38]!==w||k[39]!==b||k[40]!==S?(P=(0,n.jsxs)("div",{"data-color-mode":S,children:[w,b]}),k[38]=w,k[39]=b,k[40]=S,k[41]=P):P=k[41],P}:({ref:e,component:t,...r})=>{let[i,s]=(0,o.useState)(void 0),c=t.fields.subheading&&!u({url:t.fields.subheading.fields.href,actual:i});return(0,o.useEffect)(()=>{s(window.location.href)},[]),(0,n.jsxs)("div",{"data-color-mode":r["data-color-mode"],children:[(0,n.jsxs)(a.SubNav,{className:r.className,hasShadow:r.hasShadow,ref:e,children:[(0,n.jsx)(a.SubNav.Heading,{"data-ref":`subnav-heading-link-${t.fields.heading.sys.id}`,href:t.fields.heading.fields.href,...(0,d.$C)({action:t.fields.heading.fields.text,tag:"link",location:f}),children:t.fields.heading.fields.text}),t.fields.subheading&&(0,n.jsx)(a.SubNav.SubHeading,{"data-ref":`subnav-subheading-link-${t.fields.subheading.sys.id}`,href:t.fields.subheading.fields.href,...u({url:t.fields.subheading.fields.href,actual:i})?{"aria-current":"page"}:{},...(0,d.$C)({action:t.fields.subheading.fields.text,tag:"link",location:f}),children:t.fields.subheading.fields.text}),t.fields.links.map(e=>{if((0,l.P)(e)){let{href:t,text:s}=e.fields,o=u({url:t,actual:i});return(0,n.jsx)(a.SubNav.Link,{"data-ref":`subnav-link-${e.sys.id}`,href:t,variant:r.linkVariant,...o?{"aria-current":"page"}:{},...(0,d.$C)({action:s,tag:"link",location:f}),children:s},e.sys.id)}let{heading:t,links:s=[]}=e.fields;return(0,n.jsxs)(a.SubNav.Link,{href:t.fields.href,variant:r.linkVariant,...u({url:t.fields.href,actual:i})?{"aria-current":"page"}:{},children:[t.fields.text,s.length>0&&(0,n.jsx)(a.SubNav.SubMenu,{children:s.map(e=>(0,n.jsx)(a.SubNav.Link,{href:e.fields.href,variant:r.linkVariant,"data-ref":`subnav-submenu-link-${e.sys.id}`,...u({url:e.fields.href,actual:i})?{"aria-current":"page"}:{},...(0,d.$C)({action:e.fields.text,tag:"link",context:t.fields.text,location:f}),children:e.fields.text},e.sys.id))})]},t.sys.id)}),void 0!==t.fields.cta&&(0,n.jsx)(a.SubNav.Action,{href:t.fields.cta.fields.href,"data-ref":`subnav-cta-action-${t.fields.cta.sys.id}`,children:t.fields.cta.fields.text})]}),c&&(0,n.jsx)(a.Box,{paddingBlockStart:{narrow:64,regular:"none"}})]})};v.displayName="ContentfulSubnav"},578191(e,t,r){let n;var i=r(296540);function s(){return n||(n=window.matchMedia("(prefers-reduced-motion: reduce)"))}function a(){return!1}function o(){return s().matches}function l(e){let t=s();return t.addEventListener("change",e),()=>{t.removeEventListener("change",e)}}function c(){return(0,i.useSyncExternalStore)(l,o,a)}r.d(t,{j:()=>c})}}]);
//# sourceMappingURL=35415-af868d70a4a5c55d-0548c68c50c60a55.js.map